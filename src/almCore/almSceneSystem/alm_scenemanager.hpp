#ifndef _ALM_SCENE_MANAGER_INTERFACE_HPP_
#define _ALM_SCENE_MANAGER_INTERFACE_HPP_

#include <vector>
#include <string>
#include <functional>
#include "../src/almCore/alm_engsystem.hpp"

namespace alme
{
class AlmGameScene;
class AlmSceneManager : protected AlmEngineSystem
{
using CreateCallback = std::pair<std::string, std::function<AlmGameScene*()>>;
public:
	AlmSceneManager(AlmostEngine *engine);
	~AlmSceneManager();

	virtual void OnUpdate();
	virtual void PostUpdate();
	virtual void RunScene(uint32_t id);
	virtual void RunScene(const std::string &name);

private:
	void RunGameScene(AlmGameScene *scene);

private:
	AlmGameScene *m_activeScene;
	std::vector<CreateCallback> m_scenes;

public:
	template<class Scene> inline void AddScene(const std::string &name)
	{
		m_scenes.push_back(CreateCallback(name, []() -> AlmGameScene* { return new Scene(); }));
	}

};

}
#endif // !_ALM_SCENE_MANAGER_INTERFACE_HPP_