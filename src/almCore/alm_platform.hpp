#ifndef _ALM_PLATFORM_HPP_
#define _ALM_PLATFORM_HPP_

#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <functional>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define NOMINMAX
	#include <Windows.h>

	#define ALM_OS_WINDOWS 1

	namespace std
	{
#ifdef UNICODE
		#define ALM_UNICODE
		using almstring = wstring;
#else
		using almstring = string;
#endif
	}

#else
	#define ALM_OS_UNIX 1
#endif

#endif // !_ALM_PLATFORM_HPP_