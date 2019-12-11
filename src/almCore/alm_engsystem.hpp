#ifndef _ALM_ENGINE_SYSTEM_BASE_HPP_
#define _ALM_ENGINE_SYSTEM_BASE_HPP_

namespace alme
{

class AlmostEngine;
class AlmEngineSystem
{
public:
	AlmEngineSystem(AlmostEngine *engine) 
		: m_engine(engine) {};

	virtual ~AlmEngineSystem() {};

protected:
	virtual AlmostEngine * Engine() final
	{ return m_engine; }

private:
	AlmostEngine * m_engine;

};
}

#endif // !_ALM_ENGINE_SYSTEM_BASE_HPP_