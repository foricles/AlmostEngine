#ifndef _ALM_SCENE_INTERFACE_HPP_
#define _ALM_SCENE_INTERFACE_HPP_

#include "../src/almCore/alm_platform.hpp"

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
	void						ReleaseEntity(IAlmEntity* entity);
	IAlmEntity*					FindByName(const std::string& name) const;

private:
	void SetEnginePtr(AlmostEngine * engine);

private:
	std::string m_name;
	AlmostEngine * m_engine;

};

}
#endif // _ALM_SCENE_INTERFACE_HPP_