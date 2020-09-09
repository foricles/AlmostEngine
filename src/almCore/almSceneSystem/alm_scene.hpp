#ifndef _ALM_SCENE_INTERFACE_HPP_
#define _ALM_SCENE_INTERFACE_HPP_

#include "../src/almCore/alm_platform.hpp"

namespace alme
{
class AlmEntity;
class AlmostEngine;
class AlmGameSceneP;
class AlmTransform;
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

	const AlmostEngine*			Engine() const;
	AlmEntity *					CreateEntity(const std::string &name);
	AlmEntity *					CreateEntity(const std::string &name, AlmTransform* parent);
	void						ReleaseEntity(AlmEntity* entity);
	AlmEntity*					FindByName(const std::string& name) const;

	AlmTransform* GetRoot();
	void UpdateTransformationTree();

private:
	void SetEnginePtr(AlmostEngine * engine);

private:
	std::string m_name;
	AlmostEngine * m_engine;
	AlmTransform* m_rootTransform;
};

}
#endif // _ALM_SCENE_INTERFACE_HPP_