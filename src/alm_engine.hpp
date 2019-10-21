#ifndef _ALM_ENGINE_HPP_
#define _ALM_ENGINE_HPP_

#include "almCore/almUser/almWindow/alm_iwindow.hpp"
#include "almCore/almRender/interface/alm_irendersys.hpp"


namespace alme
{
class AlmSceneManager;
class IAlmEntityManager;

class AlmostEngine : public AlmEventHandler
{
	friend class AlmEngineEntryPoint;
public:
	AlmostEngine();
	~AlmostEngine();

private:
	void InititalizeSubsystems();
	void RunLoop();

private:
	AlmSceneManager *m_sceneManager;
	IAlmRenderSystem *m_renderSystem;
	IAlmEntityManager *m_entityManager;

	IAlmWindow *m_mainWindow;

private:
	bool m_quit;

	std::function<AlmSceneManager*()> m_sceneManagerInitializer;
	std::function<IAlmRenderSystem*()> m_renderSystemInitializer;
	std::function<IAlmEntityManager*()> m_entityManagerInitializer;

	std::function<IAlmWindow*()> m_mainWindowInitializer;

public:
	template<class T> inline void SetSceneManager()
	{
		m_sceneManagerInitializer = [this]() -> AlmSceneManager* { return new T(); };
	}
	template<class T> inline void SetEntityManager()
	{
		m_entityManagerInitializer = [this]() -> IAlmEntityManager* { return new T(); };
	}
	template<class T> inline void SetRenderSystem()
	{
		m_renderSystemInitializer = [this]() -> IAlmRenderSystem* { return new T(); };
	}
	template<class T> inline void SetMainWindow()
	{
		m_mainWindowInitializer = [this]() -> IAlmWindow* { return new T(); };
	}

};
}

#endif // !_ALM_ENGINE_HPP_