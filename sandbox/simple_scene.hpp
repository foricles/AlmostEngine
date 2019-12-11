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
#include <almCore/almRender/openglrender/glmaterial/alm_glmaterial.hpp>
#include <almCore/almRender/alm_mesh.hpp>
#include <almCore/almUtils/tinyxml/tinyxml2.h>

class SimpleScene : public alme::AlmGameScene, public alme::AlmEventHandler
{
public:
	void OnStart() override
	{
		tinyxml2::XMLDocument doc;
		doc.Parse("");

		std::string name = Engine()->GetRenderSystem().GetGpuVendorName();

		CreateEntity("pidor");
		CreateEntity("suka");
		CreateEntity("mraz");

		int a = 7;
	}
};