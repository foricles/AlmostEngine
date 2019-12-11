#pragma once

#include <almCore/almFileSystem/alm_file.hpp>
#include <almCore/almEntitySystem/alm_entity.hpp>
#include <almCore/almEntitySystem/alm_entitymgr.hpp>
#include <iostream>
#include "almCore/almSceneSystem/alm_iscene.hpp"
#include <almCore/almEvents/alm_eventhandler.hpp>
#include <almCore/almMath/factorial.hpp>
#include <almCore/almRender/alm_camera.hpp>
#include <almCore/almFileSystem/alm_flsystem.hpp>
#include <almCore/almRender/openglrender/glmaterial/alm_glmaterial.hpp>
#include <almCore/almRender/alm_mesh.hpp>

class SimpleScene : public alme::IAlmScene, public alme::AlmEventHandler
{
public:
	void OnStart() override
	{

	}
};