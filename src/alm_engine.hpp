#ifndef _ALM_ENGINE_HPP_
#define _ALM_ENGINE_HPP_

#include <functional>


namespace alme
{
class AlmSceneManager;
class IAlmRenderSystem;
class IAlmEntityManager;

class AlmostEngine
{
	friend class AlmEngineEntryPoint;
public:
	AlmostEngine();
	virtual ~AlmostEngine();

private:
	void InititalizeSubsystems();
	void RunLoop();

private:
	AlmSceneManager *m_sceneManager;
	IAlmRenderSystem *m_renderSystem;
	IAlmEntityManager *m_entityManager;

private:
	std::function<AlmSceneManager*()> m_sceneManagerInitializer;
	std::function<IAlmRenderSystem*()> m_renderSystemInitializer;
	std::function<IAlmEntityManager*()> m_entityManagerInitializer;

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

};
}

#endif // !_ALM_ENGINE_HPP_