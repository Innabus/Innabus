#include "ibFileSystem.h"

#include "ibCriticalSection.h"
#include "ibFileRef.h"
#include "ibHash.h"
#include "ibOpenFileJob.h"
#include "ibUtil.h"

#include "PathCch.h"

#include <map>

namespace
{
	ibFileSystem* s_pFileSystem;

	const char* s_pathGameBase = 0;
	const char* s_pathExtensions[kPath_Count] = {
		"", // Game base
		"data\\", // Data
		"audio\\", // Audio
		"music\\", // Music
		"save\\", // UserSave
		"screenshots\\", // screenshots
		"script\\", // Scripts
		"script\\", // UserScripts
		"mod\\", // Mods
	};

	void SysFileEnum(const char* path, ibFileEnumCallback callback, void* pUserData, int attrPattern)
	{
		WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFileA(path, &ffd);

		ibAssert(INVALID_HANDLE_VALUE != hFind);
		if (INVALID_HANDLE_VALUE == hFind)
			return;
		
		do
		{
			if (ffd.dwFileAttributes & attrPattern)
				callback(pUserData, ffd.cFileName);
		}
		while (FindNextFileA(hFind, &ffd) != 0);

		ibAssert(GetLastError() == ERROR_NO_MORE_FILES);
		SetLastError(0);

		FindClose(hFind);
	}

	const char* SysPathSpec(const char* path)
	{
		u32 len = strlen(path) + 1;
		int req = MultiByteToWideChar(CP_UTF8, 0, path, len, 0, 0) + 1;
		wchar_t* pWidePath = new ("SysPathSpec tmp wstr") wchar_t[req];
		MultiByteToWideChar(CP_UTF8, 0, path, len, pWidePath, req);
		PathCchRemoveFileSpec(pWidePath, req);
		PathCchAddBackslash(pWidePath, req);
		len = wcslen(pWidePath) + 1;
		req = WideCharToMultiByte(CP_UTF8, 0, pWidePath, len, 0, 0, NULL, NULL);
		char* out = new ("SysPathSpec result") char[req];
		WideCharToMultiByte(CP_UTF8, 0, pWidePath, len, out, req, NULL, NULL);
		delete [] pWidePath;
		return out;
	}

	typedef ibMap<u32, ibFileRef*> FileRefMap;
	FileRefMap* s_pFileMap = 0;
#define g_fileMap (*s_pFileMap)

	ibCriticalSection s_fileSystemLock;

	ibFileRef SysOpenFile(const char* filename, bool writable)
	{
		u32 hash = ibHash(filename, strlen(filename));

		FileRefMap::iterator iter = g_fileMap.find(hash);
		if (iter == g_fileMap.end())
			g_fileMap[hash] = new ("File Ref") ibFileRef;

		ibFileRef ret = *g_fileMap[hash];

		g_fileSystem.Lock();
		if (!ret.GetState())
			g_mcp.QueueJob(new ("Open file job") ibOpenFileJob(filename, writable, ret));
		g_fileSystem.Unlock();

		return ret;
	}
}

ibFileSystem::ibFileSystem()
{
	s_pFileMap = new (g_engineHeap->AllocHigh(sizeof(FileRefMap), "FileRefMap")) FileRefMap;
}

ibFileSystem::~ibFileSystem()
{
	//FileRefMap::iterator iter, e = g_fileMap.end();
	//for (iter = g_fileMap.begin(); iter != e; ++iter)
	//{
	//	CloseFile(iter->second)
	//}
	ibAssert(g_fileMap.size() == 0);
	delete s_pFileMap;
	delete [] s_pathGameBase;
	s_pathGameBase = 0;
}

ibFileSystem* ibFileSystem::Get()
{
	if (!s_pFileSystem)
		s_pFileSystem = new (g_engineHeap->AllocHigh(sizeof(ibFileSystem), "File System")) ibFileSystem;
	return s_pFileSystem;
}

void ibFileSystem::Shutdown()
{
	delete s_pFileSystem;
	s_pFileSystem = 0;
}

ibFileRef ibFileSystem::OpenDataFile(const char* filename)
{
	return SysOpenFile(filename, false);
}

ibFileRef ibFileSystem::OpenUserFile(const char* filename)
{
	return SysOpenFile(filename, true);
}

void ibFileSystem::CloseFile(ibFileRef* ref)
{
	// See ~ibFileRef
	ibAssert(ref->GetRefCount() == 2);
	ibFileInfo* pInfo = ref->Get();

	Lock();
	pInfo->m_state = kFile_Closed;

	FileRefMap::iterator iter, e = g_fileMap.end();
	for (iter = g_fileMap.begin(); iter != e; ++iter)
	{
		if ((*(iter->second))->m_file == pInfo->m_file)
		{
			delete iter->second;
			g_fileMap.erase(iter);
			break;
		}
	}

	CloseHandle(pInfo->m_file);
	pInfo->m_file = INVALID_HANDLE_VALUE;

	Unlock();
}

void ibFileSystem::EnumFiles(const char* path, ibFileEnumCallback callback, void* pUserData)
{
	char usePath[MAX_PATH] = { 0 };
	int len = strlen(path);
	if (len >= MAX_PATH - 1)
		return;

	strncpy(usePath, path, MAX_PATH);
	if (usePath[len - 1] == '\\')
		usePath[len] = '*';

	SysFileEnum(usePath, callback, pUserData, ~FILE_ATTRIBUTE_DIRECTORY);
}

void ibFileSystem::EnumDirectories(const char* path, ibFileEnumCallback callback, void* pUserData)
{
	char usePath[MAX_PATH] = { 0 };
	int len = strlen(path);
	if (len >= MAX_PATH - 1)
		return;

	strncpy(usePath, path, MAX_PATH);
	if (usePath[len - 1] == '\\')
		usePath[len] = '*';

	SysFileEnum(usePath, callback, pUserData, FILE_ATTRIBUTE_DIRECTORY);
}

#pragma warning(push)
#pragma warning(disable : 4127) // Conditional is constant
const char* ibFileSystem::GetStandardPath(ibStandardPath stdPath)
{
	const char* pPath = 0;
	char* tmp = 0;

	if (!s_pathGameBase)
	{
		tmp = new ("Standard path result") char[1024];
		GetModuleFileNameA(NULL, tmp, 1024);
		pPath = SysPathSpec(tmp);
		s_pathGameBase = pPath;
		delete [] tmp;
	}

	switch (stdPath)
	{
	case kPath_GameBase:
	case kPath_Data:
	case kPath_Audio:
	case kPath_Music:
	case kPath_UserSave:
	case kPath_ScreenShots:
	case kPath_Scripts:
	case kPath_UserScripts:
	case kPath_Mods:
		tmp = new ("Path") char[strlen(s_pathGameBase) + strlen(s_pathExtensions[stdPath]) + 1];
		strcpy(tmp, s_pathGameBase);
		strcat(tmp, s_pathExtensions[stdPath]);
		pPath = tmp;
		break;

	default:
		ibAssertMsg(false, "Invalid path value");
		break;
	}

	return pPath;
}
#pragma warning(pop)

bool ibFileSystem::HasFileChanged(const char* filename)
{
	u32 hash = ibHash(filename, strlen(filename));
	return HasFileChanged(hash);
}

bool ibFileSystem::HasFileChanged(const u32 pathHash)
{
	FileRefMap::const_iterator iter = g_fileMap.find(pathHash);
	if (iter == g_fileMap.end())
		return false;
	return HasFileChanged(*iter->second);
}

bool ibFileSystem::HasFileChanged(const ibFileRef& ref)
{
	if (ref.Get()->m_file == INVALID_HANDLE_VALUE)
		return false;

	FILETIME ft = { 0 };
	GetFileTime(ref.Get()->m_file, NULL, NULL, &ft);
	return 1 == CompareFileTime(&ref.Get()->m_openedTime, &ft);
}

bool ibFileSystem::CanReadAnywhere()
{
	return true;
}

bool ibFileSystem::CanWriteOutsideUserData()
{
	return true;
}

void ibFileSystem::Lock()
{
	s_fileSystemLock.Lock();
}

void ibFileSystem::Unlock()
{
	s_fileSystemLock.Release();
}
