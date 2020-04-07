#ifndef _ALM_SCENE_INTERFACE_HPP_
#define _ALM_SCENE_INTERFACE_HPP_

#include "../src/almCore/alm_platform.hpp"

namespace alme
{
class IAlmEntity;
class AlmostEngine;
class AlmGameSceneP;
class IAlmTransform;
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
	IAlmEntity *				CreateEntity(const std::string &name, IAlmTransform* parent);
	void						ReleaseEntity(IAlmEntity* entity);
	IAlmEntity*					FindByName(const std::string& name) const;

	IAlmTransform* GetRoot();
	void UpdateTransformationTree();

private:
	void SetEnginePtr(AlmostEngine * engine);

private:
	std::string m_name;
	AlmostEngine * m_engine;
	IAlmTransform* m_rootTransform;
};

}
#endif // _ALM_SCENE_INTERFACE_HPP_