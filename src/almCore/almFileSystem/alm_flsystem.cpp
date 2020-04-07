#include "alm_flsystem.hpp"
#include <locale>
#include <cstdio>
#include <codecvt>

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

std::wstring io::AlmFileSystem::StrToWStr(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(str);
}

std::string io::AlmFileSystem::WStrToStr(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}