#include <fstream>
#include <cstdlib>
#include <string>
//#include <list>

#include "ibFileSystem.h"
#include "ibFile.h"
#include "ibMemory.h"
#include "ibStd.h"

#include <Windows.h>

std::ofstream fout = std::ofstream("out.txt");

void WriteHeader(const std::string& s)
{
	fout << "\\\\\n\\\\ " << s << "\n\\\\" << std::endl;
}

void WriteStandardPaths()
{
	WriteHeader("Standard Paths");
	for (u32 n = 0; n < kPath_Count; ++n)
	{
		const char* path = g_fileSystem.GetStandardPath(ibStandardPath(n));
		fout << path << std::endl;
		delete [] path;
	}
}

typedef ibList<std::string> PathList;

void EnumCallback(void* pUser, const char* path)
{
	PathList* pList = (PathList*)pUser;
	pList->push_back(path);
}

void EnumFiles(ibStandardPath pathKey)
{
	PathList paths;
	const char* pathBase = g_fileSystem.GetStandardPath(pathKey);
	g_fileSystem.EnumFiles(pathBase, EnumCallback, &paths);
	for (auto p : paths)
		fout << p << std::endl;
	delete [] pathBase;
}

void EnumDirs()
{
	WriteHeader("EnumDirectories");
	PathList paths;
	const char* pathBase = g_fileSystem.GetStandardPath(kPath_GameBase);
	g_fileSystem.EnumDirectories(pathBase, EnumCallback, &paths);
	for (auto p : paths)
		fout << p << std::endl;
	delete [] pathBase;
}

ibFile<char> TestOpenData()
{
	WriteHeader("Open Data File");
	const char* cpath = g_fileSystem.GetDataPath();
	std::string filename = cpath;
	filename += "data0.dat";
	ibFileRef ref = g_fileSystem.OpenDataFile(filename.c_str());
	while (!ref.IsOpen()) {}
	fout << "File opened" << std::endl;
	delete [] cpath;
	return ref.ReadAs<char>();
}

ibFile<char> TestOpenUser(u32 writeSize)
{
	WriteHeader("Open User File");
	const char* cpath =  g_fileSystem.GetUserPath();
	std::string filename = cpath;
	filename += "user0.dat";
	ibFileRef ref = g_fileSystem.OpenUserFile(filename.c_str());
	while (!ref.IsOpen()) {}
	fout << "Writable: " << ref.IsWritable() << std::endl;
	delete [] cpath;
	return ref.Resize<char>(writeSize);
}

extern "C"
int GameMain()
{
	WriteStandardPaths();

	EnumDirs();
	WriteHeader("EnumFiles: kPath_Data");
	EnumFiles(kPath_Data);
	WriteHeader("EnumFiles: kPath_UserSave");
	EnumFiles(kPath_UserSave);

	{
		ibFile<char> inFile = TestOpenData();
		ibFile<char> outFile = TestOpenUser(inFile.Size());
		while (!inFile.IsReady()) {}
		if (!outFile.IsReady())
			fout << "Outfile ready check failed!" << std::endl;
		else
		{
			ibMemcpy(outFile.GetData(), inFile.GetData(), outFile.Size());
			outFile.WriteFile();
		}
		while (!outFile.IsReady()) {}
	}

	fout.flush();
	Sleep(1000);
	fout.close();
	
	return 0;
}

extern "C"
int GameUpdate(float)
{
	return 0;
}
