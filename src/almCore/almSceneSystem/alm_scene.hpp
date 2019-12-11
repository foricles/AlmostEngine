#ifndef _ALM_SCENE_INTERFACE_HPP_
#define _ALM_SCENE_INTERFACE_HPP_

#include <string>

namespace alme
{
class IAlmEntity;
class AlmostEngine;
class AlmGameSceneP;
class AlmGameScene
{
friend class AlmSceneManager;
public:
	AlmGameScene();
	virtual ~AlmGameScene();


	virtual void				OnStart() {};
	virtual void				OnUpdate() {};
	virtual void				PostUpdate() {};
	virtual void				OnDelete() {};

	const AlmostEngine *		Engine() const;
	IAlmEntity *				CreateEntity(const std::string &name);

private:
	void SetEnginePtr(AlmostEngine * engine);

private:
	std::string m_name;
	AlmostEngine * m_engine;
	AlmGameSceneP * m_scene;

};

}
#endif // _ALM_SCENE_INTERFACE_HPP_