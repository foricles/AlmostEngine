#ifndef _ALM_SCENE_MANAGER_INTERFACE_HPP_
#define _ALM_SCENE_MANAGER_INTERFACE_HPP_

#include <vector>
#include <string>
#include <functional>

namespace alme
{
class IAlmScene;

using CreateCallback = std::pair<std::string, std::function<IAlmScene*()>>;

class AlmSceneManager
{
public:
	AlmSceneManager();
	virtual ~AlmSceneManager();

	virtual void RunScene(const std::string &name);
	virtual void RunScene(uint32_t id);
	virtual void OnUpdate();
	virtual void PostUpdate();

private:
	IAlmScene *m_activeScene;
	std::vector<CreateCallback> m_scenes;

public:
	template<class Scene> inline void AddScene(const std::string &name)
	{
		m_scenes.push_back(CreateCallback(name, []() -> IAlmScene* { return new Scene(); }));
	}

};

}
#endif // !_ALM_SCENE_MANAGER_INTERFACE_HPP_