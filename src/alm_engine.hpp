#ifndef _ALM_ENGINE_HPP_
#define _ALM_ENGINE_HPP_

#include <functional>


namespace alme
{
class AlmSceneManager;
class IAlmEntityManager;

class AlmostEngine
{
	friend class AlmEngineEntryPoint;
public:
	AlmostEngine();
	virtual ~AlmostEngine();

private:
	AlmSceneManager *m_sceneManager;
	IAlmEntityManager *m_entityManager;

private:
	std::function<AlmSceneManager*()> m_sceneManagerInitializer;
	std::function<IAlmEntityManager*()> m_entityManagerInitializer;

public:
	template<class T> inline void SetSceneManager()
	{
		m_sceneManagerInitializer = []() -> AlmSceneManager* { return new T(); };
	}
	template<class T> inline void SetEntityManager()
	{
		m_entityManagerInitializer = []() -> IAlmEntityManager* { return new T(); };
	}

};
}

#endif // !_ALM_ENGINE_HPP_