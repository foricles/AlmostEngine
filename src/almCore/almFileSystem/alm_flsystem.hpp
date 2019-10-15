#ifndef _ALM_FILE_SYSTEM_HPP_
#define _ALM_FILE_SYSTEM_HPP_

#include "../alm_platform.hpp"

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

private:

};


}
}

#endif // !_ALM_FILE_SYSTEM_HPP_