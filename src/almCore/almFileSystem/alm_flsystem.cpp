#include "alm_flsystem.hpp"
#include <cstdio>

using namespace alme;

io::AlmFileSystem::AlmFileSystem()
{
}

io::AlmFileSystem::~AlmFileSystem()
{
}

std::almstring io::AlmFileSystem::getRootPath()
{
#ifdef ALM_OS_WINDOWS
#ifdef ALM_UNICODE
	WCHAR exe_path[_MAX_PATH + 1];
#else
	CHAR exe_path[_MAX_PATH + 1];
#endif // UNICODE

	GetModuleFileName(GetModuleHandleW(NULL), exe_path, _MAX_PATH);
	std::almstring finalPath(exe_path);

#ifdef ALM_UNICODE
	return finalPath.substr(0, finalPath.find_last_of(L"\\")+1);
#else
	return finalPath.substr(0, finalPath.find_last_of("\\")+1);
#endif // UNICODE

#elif ALM_OS_UNIX

#endif // ALM_OS_WINDOWS
}
