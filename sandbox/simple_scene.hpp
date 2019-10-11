#pragma once

#include <almCore/almFileSystem/alm_file.hpp>
#include <almCore/almEntitySystem/almEntity/alm_entity.hpp>
#include <almCore/almEntitySystem/alm_entitymgr.hpp>
#include <almCore/almEntitySystem/almEntity/alm_entity.hpp>
#include <iostream>
#include "almCore/almSceneSystem/alm_iscene.hpp"

class SimpleScene : public alme::IAlmScene
{
public:
	void OnStart() override
	{
		std::cout << sizeof(alme::io::AlmFile) << std::endl;
		std::wcout << alme::io::AlmFileSystem::getRootPath() << std::endl;
		alme::io::AlmFile sss(L"Sandbox.pdb");
		//std::wcout << sss.GetFullPath()  << std::endl << (sss.Exist() ? sss.Load() : "hui") << std::endl;
		std::cout << "start load" << std::endl;
		sss.LoadAsync();
		std::wcout << sss.GetContentAsync() << std::endl;

		alme::AlmEntityManager mgr;
		mgr.CreateEntity("Pidor");
		mgr.CreateEntity("Mraz");
		mgr.CreateEntity("Suka");
		mgr.CreateEntity("Zhopa");

		alme::AlmEntity *entity = static_cast<alme::AlmEntity*>(mgr.FindByName("Zhopa"));
		system("pause");
	}
};