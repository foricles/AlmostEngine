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

		alme::AlmEntity *entity1 = static_cast<alme::AlmEntity*>(mgr.FindByName("Zhopa"));
		alme::AlmEntity *entity2 = static_cast<alme::AlmEntity*>(mgr.FindByName("Pidor"));

		entity1->GetTransform().SetParent(entity2->GetTransform());
		entity1->GetTransform().SetPosition(5, 6, 7);

		entity2->GetTransform().SetPosition(10, 20, 30);
		entity2->GetTransform().SetScale(10, -3, 0);
		entity2->GetTransform().UpdateModelMatrix();

		std::cout << entity1->GetTransform().GetPosition().toString() << std::endl;

		std::cout << entity2->GetTransform().GetModelMatrix().toString() << std::endl;
		std::cout << entity1->GetTransform().GetModelMatrix().toString() << std::endl;

		auto sz = sizeof(alme::AlmEntity);
	}
};