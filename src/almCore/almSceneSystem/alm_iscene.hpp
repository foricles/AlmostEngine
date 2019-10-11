#ifndef _ALM_SCENE_INTERFACE_HPP_
#define _ALM_SCENE_INTERFACE_HPP_

#include <string>

namespace alme
{

class IAlmScene
{
public:
	virtual ~IAlmScene() {}

	virtual void OnStart() {};
	virtual void OnDelete() {};

private:
	std::string m_name;
};

}
#endif // _ALM_SCENE_INTERFACE_HPP_