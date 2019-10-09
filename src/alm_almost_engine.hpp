#ifndef _ALM_ALMOST_ENGINE_HPP_
#define _ALM_ALMOST_ENGINE_HPP_

#include "alm_engine.hpp"

extern AlmEngineEntryPoint *CreateEntryPoint();
int main(int argc, char **argv)
{
	AlmEngineEntryPoint *entry_point = CreateEntryPoint();
	entry_point->InitializeSsettings();
	entry_point->RegisterGameScenes();
	delete entry_point;

	return 0;
}

#endif // !_ALM_ALMOST_ENGINE_HPP_
