#ifndef _ALM_FILE_SYSTEM_HPP_
#define _ALM_FILE_SYSTEM_HPP_

#include "../src/almCore/alm_platform.hpp"

namespace alme
{
namespace io
{

class AlmFileSystem
{
public:
	AlmFileSystem();
	~AlmFileSystem();

	static std::almstring getRootPath();
	static std::wstring StrToWStr(const std::string & str);
	static std::string WStrToStr(const std::wstring & wstr);
};


}
}

#endif // !_ALM_FILE_SYSTEM_HPP_