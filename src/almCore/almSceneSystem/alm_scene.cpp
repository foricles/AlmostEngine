#include "alm_scene.hpp"
#include "../src/alm_engine.hpp"
#include "../src/almCore/almUser/almWindow/alm_iwindow.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_imgr.hpp"
#include "../src/almCore/almRender/interface/alm_irendersys.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_ientity.hpp"
#include "../src/almCore/almEntitySystem/interface/alm_itransform.hpp"

namespace alme
{
	class AlmGameSceneP
	{
	public:
		IAlmTransform * m_root = nullptr;
	};
};


using namespace alme;

AlmGameScene::AlmGameScene()
	: m_engine(nullptr)
	, m_scene(new AlmGameSceneP())
{

}

AlmGameScene::~AlmGameScene()
{
	delete m_scene;
}

void AlmGameScene::SetEnginePtr(AlmostEngine * engine)
{
	m_engine = engine;
}

const AlmostEngine * AlmGameScene::Engine() const
{
	return m_engine;
}

IAlmEntity * AlmGameScene::CreateEntity(const std::string & name)
{
	IAlmEntity * newEntity = const_cast<IAlmEntityManager*>(&m_engine->GetEntityManager())->CreateEntity(name);

	if (m_scene->m_root != nullptr)
		newEntity->GetTransform()->AddChild(m_scene->m_root);
	m_scene->m_root = newEntity->GetTransform();

	return newEntity;
}
