#ifndef _ALM_ENGINE_HPP_
#define _ALM_ENGINE_HPP_

class AlmEngineEntryPoint
{
public:
	AlmEngineEntryPoint();
	virtual ~AlmEngineEntryPoint();

	virtual void InitializeSsettings() = 0;
	virtual void RegisterGameScenes() = 0;

protected:

};

#endif // !_ALM_ENGINE_HPP_