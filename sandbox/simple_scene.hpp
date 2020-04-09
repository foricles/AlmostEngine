#pragma once

#include <almCore/almFileSystem/alm_file.hpp>
#include <almCore/almEntitySystem/alm_entity.hpp>
#include <almCore/almEntitySystem/alm_entitymgr.hpp>
#include <iostream>
#include "almCore/almSceneSystem/alm_scene.hpp"
#include <almCore/almEvents/alm_eventhandler.hpp>
#include <almCore/almMath/factorial.hpp>
#include <almCore/almRender/alm_camera.hpp>
#include <almCore/almFileSystem/alm_flsystem.hpp>
#include <almCore/almRender/openglrender/alm_glmaterial.hpp>
#include <almCore/almRender/openglrender/alm_glmesh.hpp>
#include <almCore/almUtils/tinyxml/tinyxml2.h>
#include <almCore/almUtils/alm_timesystem.hpp>
#include <almCore/almUtils/image/alm_imageloader.hpp>

class SimpleScene : public alme::AlmGameScene, public alme::AlmEventHandler
{
public:
	void OnStart() override
	{
		tinyxml2::XMLDocument doc;
		doc.Parse("");

		std::string name = Engine()->GetRenderSystem().GetGpuVendorName();

		for (int i(0); i <5 ; ++i)
		{
			auto ent = CreateEntity("i_" + std::to_string(i));
			//ent->onUpdate.Add<SimpleScene>(this, &SimpleScene::update);
			for (int j(0); j < 6; ++j)
			{
				auto ent2 = CreateEntity("i_" + std::to_string(i) + "j_" + std::to_string(j));
				//ent2->onUpdate.Add<SimpleScene>(this, &SimpleScene::update);
				ent->GetTransform()->AddChild(ent2->GetTransform());
				for (int k(0); k < 7; ++k)
				{
					auto ent3 = CreateEntity("i_" + std::to_string(i) + "j_" + std::to_string(j) + "k_" + std::to_string(k));
					//ent3->onUpdate.Add<SimpleScene>(this, &SimpleScene::update);
					ent2->GetTransform()->AddChild(ent3->GetTransform());
				}
			}
		}

		CreateEntity("updateentity")->onUpdate.Add<SimpleScene>(this, &SimpleScene::update);

		std::cout << Engine()->GetEntityManager().EntitiesCount() << " entities" << std::endl;
		std::cout << Engine()->GetEntityManager().AllocatedMemory() << " bytes" << std::endl;

		std::cout << alme::io::AlmFileSystem::WStrToStr(alme::io::AlmFileSystem::getRootPath()) << std::endl;
		alme::io::AlmFile file("sha.shader");
		std::cout << alme::io::AlmFileSystem::WStrToStr(file.GetFullPath()) << std::endl;
		file.Load();
		std::cout << file.asString() << std::endl;

		auto image = alme::image::AlmImageLoader::Create("test.png");
		alme::kmu::vec2 m(image->GetWight()/2.0f, image->GetHeight()/2.0f);
		float n = 500;
		float dt = ((std::atan(1.0) * 8) / n);
		for (int i(0); i < n; ++i)
			image->SetPixel(cos(i*dt)*100+m.x, sin(i*dt)*100+m.y, {0,0,0,255});
		alme::image::AlmImageLoader::SaveImage(image, "procesed.png");
		int a = 5;

		auto qer = alme::image::AlmImageLoader::Create(10, 10, alme::image::ePng);
		qer->SetPixel(9, 0, {0,0,0,255});
		qer->SetPixel(0, 9, {255,0,0,255});
		alme::image::AlmImageLoader::SaveImage(qer, "created.png");
	}

	void update(alme::IAlmEntity* e)
	{
		std::cout << e->GetName() << std::endl;
		for (const auto & t : Engine()->GetTimeSystem().timePoints())
			std::cout << t.first << ": " << t.second << std::endl;
		std::cout << "---------------------------" << std::endl;
	}
};