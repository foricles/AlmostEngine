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
			ent->onUpdate.Add<SimpleScene>(this, &SimpleScene::update);
			for (int j(0); j < 6; ++j)
			{
				auto ent2 = CreateEntity("i_" + std::to_string(i) + "j_" + std::to_string(j));
				ent2->onUpdate.Add<SimpleScene>(this, &SimpleScene::update);
				ent->GetTransform()->AddChild(ent2->GetTransform());
				for (int k(0); k < 7; ++k)
				{
					auto ent3 = CreateEntity("i_" + std::to_string(i) + "j_" + std::to_string(j) + "k_" + std::to_string(k));
					ent3->onUpdate.Add<SimpleScene>(this, &SimpleScene::update);
					ent2->GetTransform()->AddChild(ent3->GetTransform());
				}
			}
		}
	}

	void update(alme::IAlmEntity* e)
	{
		std::cout << e->GetName() << std::endl;
	}
};