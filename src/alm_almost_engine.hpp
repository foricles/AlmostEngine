#ifndef _ALM_ALMOST_ENGINE_HPP_
#define _ALM_ALMOST_ENGINE_HPP_

#include "alm_entry_point.hpp"

extern alme::AlmEngineEntryPoint *CreateEntryPoint();
int main(int argc, char **argv)
{
	alme::AlmEngineEntryPoint *entry_point = CreateEntryPoint();
	entry_point->Run();
	delete entry_point;

	return 0;
}

#endif // !_ALM_ALMOST_ENGINE_HPP_
