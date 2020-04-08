#ifndef _ALM_ENGINE_HPP_
#define _ALM_ENGINE_HPP_

#include "almCore/almUser/almWindow/alm_iwindow.hpp"
#include "almCore/almRender/interface/alm_irendersys.hpp"


namespace alme
{
class AlmTimeSystem;
class AlmSceneManager;
class IAlmEntityManager;

class AlmostEngine : public AlmEventHandler
{
	friend class AlmEngineEntryPoint;
public:
	AlmostEngine();
	~AlmostEngine();

	const IAlmWindow &					GetMainWindow() const;
	const AlmTimeSystem &				GetTimeSystem() const;
	const AlmSceneManager &				GetSceneManager() const;
	const IAlmRenderSystem &			GetRenderSystem() const;
	const IAlmEntityManager &			GetEntityManager() const;

private:
	void								InititalizeSubsystems();
	void								RunLoop();
	IAlmWindow							*m_mainWindow;
	AlmTimeSystem						*m_timeSystem;
	AlmSceneManager						*m_sceneManager;
	IAlmRenderSystem					*m_renderSystem;
	IAlmEntityManager					*m_entityManager;

	std::function<IAlmWindow*()>		 m_mainWindowInitializer;
	std::function<AlmSceneManager*()>	 m_sceneManagerInitializer;
	std::function<IAlmRenderSystem*()>	 m_renderSystemInitializer;
	std::function<IAlmEntityManager*()>  m_entityManagerInitializer;

private:
	bool								 m_quit;

private:
	template<class T> inline void SetSceneManager()
	{
		m_sceneManagerInitializer = [this]() -> AlmSceneManager* { return new T(this); };
	}
	template<class T> inline void SetEntityManager()
	{
		m_entityManagerInitializer = [this]() -> IAlmEntityManager* { return new T(this); };
	}
	template<class T> inline void SetRenderSystem()
	{
		m_renderSystemInitializer = [this]() -> IAlmRenderSystem* { return new T(this); };
	}
	template<class T> inline void SetMainWindow()
	{
		m_mainWindowInitializer = [this]() -> IAlmWindow* { return new T(this); };
	}
};
}

#endif // !_ALM_ENGINE_HPP_