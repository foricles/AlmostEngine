#pragma once

#include <almCore/almFileSystem/alm_file.hpp>
#include <almCore/almEntitySystem/almEntity/alm_entity.hpp>
#include <almCore/almEntitySystem/alm_entitymgr.hpp>
#include <almCore/almEntitySystem/almEntity/alm_entity.hpp>
#include <iostream>
#include "almCore/almSceneSystem/alm_iscene.hpp"
#include <almCore/almEvents/alm_eventhandler.hpp>
#include <almCore/almMath/factorial.hpp>
#include <almCore/almRender/alm_camera.hpp>
#include <almCore/almFileSystem/alm_flsystem.hpp>

class Some : public alme::AlmEventHandler
{
public:
	void test(int a, char c)
	{
		std::cout << "Some: " << a << c << std::endl;
	}

	~Some()
	{
		std::cout << "Some deleted" << std::endl;
	}
};

class SimpleScene : public alme::IAlmScene, public alme::AlmEventHandler
{
public:
	void OnStart() override
	{
		std::cout << sizeof(alme::io::AlmFile) << std::endl;
		std::wcout << alme::io::AlmFileSystem::getRootPath() << std::endl;
		alme::io::AlmFile sss("Sandbox.pdb");
		std::cout << "start load" << std::endl;
		sss.Load();
		std::cout << sss.asString() << std::endl;

		alme::AlmEntityManager mgr;
		mgr.CreateEntity("Pidor");
		mgr.CreateEntity("Mraz");
		mgr.CreateEntity("Suka");
		mgr.CreateEntity("Zhopa");

		alme::AlmEntity *entity1 = static_cast<alme::AlmEntity*>(mgr.FindByName("Zhopa"));
		alme::AlmEntity *entity2 = static_cast<alme::AlmEntity*>(mgr.FindByName("Pidor"));

		entity2->GetTransform().SetParent(entity1->GetTransform());

		entity1->GetTransform().SetPosition(5, 6, 7);
		entity1->GetTransform().SetRotation(alme::kmu::quaternion::euler(35, 1, 0, 1));

		entity2->GetTransform().SetPosition(10, 20, 30);
		entity2->GetTransform().SetScale(10, -3, 1);

		std::cout << entity2->GetTransform().GetModelMatrix().toString() << std::endl;
		std::cout << entity1->GetTransform().GetModelMatrix().toString() << std::endl;

		std::cout << entity2->GetTransform().GetPosition().toString() << " | " << entity2->GetTransform().GetLocalPosition().toString() << std::endl;

		auto sz = sizeof(alme::AlmEntity);

		alme::AlmDelegate<int, char> test;
		test.Add<SimpleScene>(this, &SimpleScene::test);
		test.Add<SimpleScene>(this, &SimpleScene::test);

		{
			Some s;
			test.Add<Some>(&s, &Some::test);
			test.Execute(1, 'C');
		}
			test.Execute(1, 'C');

			for (int i(13); i < 20; ++i)
				std::cout << alme::kmu::factorial(i)() << std::endl;

			alme::kmu::factorial a1(3), a2(5);
			std::cout << a1() << " | " << a2() << std::endl;
			std::cout << a1 / a2 << " | " << a2 / a1 << std::endl;

			alme::AlmCamera camera;
			camera.GetTransform().SetPosition(1, 2, 3);
			camera.SetPerspective(65, 0.3f, 100, 640, 480);
			std::cout << camera.GetProjectionMatrix().toString() << std::endl;
			std::cout << camera.GetCameraMatrix().toString() << std::endl;
	}

	void test(int a, char c)
	{
		std::cout << a << c << std::endl;
	}
};