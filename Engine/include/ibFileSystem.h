#ifndef IB_FILESYSTEM_H
#define IB_FILESYSTEM_H
#pragma once

class ibFileRef;

typedef void (*ibFileEnumCallback)(void* pUser, const char* path);

enum ibStandardPath
{
	kPath_GameBase,
	kPath_Data,
	kPath_Audio,
	kPath_Music,
	kPath_UserSave,
	kPath_ScreenShots,
	kPath_Scripts,
	kPath_UserScripts,
	kPath_Mods,

	kPath_Count
};

class IB_EXPORT ibFileSystem
{
public:
	static ibFileSystem* Get();
	static void Shutdown();

	ibFileRef OpenDataFile(const char* filename);
	ibFileRef OpenUserFile(const char* filename);

	void EnumFiles(const char* path, ibFileEnumCallback callback, void* pUserData);
	template <class T> void EnumFiles(const char* path, void (T::*callback)(void*, const char*), void* pUserData);
	void EnumDirectories(const char* path, ibFileEnumCallback callback, void* pUserData);
	template <class T>	void EnumDirectories(const char* path, void (T::*callback)(void*, const char*), void* pUserData);

	const char* GetDataPath() { return GetStandardPath(kPath_Data); }
	const char* GetUserPath() { return GetStandardPath(kPath_UserSave); }
	const char* GetStandardPath(ibStandardPath stdPath);

	bool HasFileChanged(const char* filename);
	bool HasFileChanged(const u32 pathHash);
	bool HasFileChanged(const ibFileRef&);
	bool CanReadAnywhere();
	bool CanWriteOutsideUserData();

	void CloseFile(ibFileRef*);

	static void Lock();
	static void Unlock();

private:
	ibFileSystem();
	~ibFileSystem();
};

#define g_fileSystem (*ibFileSystem::Get())

#endif // IB_FILESYSTEM_H
