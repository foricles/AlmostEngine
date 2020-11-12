#include "alm_rendercomponent.hpp"
#include "../src/almCore/almRender/interface/alm_imesh.hpp"
#include "../src/almCore/almRender/interface/alm_imaterial.hpp"

namespace alme
{
	AlmRenderComponent::AlmRenderComponent(AlmEntity* owner)
		: IAlmComponentImp(owner)
		, m_mesh(nullptr)
		, m_material(nullptr)
	{
	}
	AlmRenderComponent::~AlmRenderComponent()
	{
	}



	void AlmRenderComponent::SetMesh(IAlmMesh* mesh)
	{
		m_mesh = mesh;
	}

	void AlmRenderComponent::SetMaterial(IAlmRenderMaterial* material)
	{
		m_material = material;
	}

	IAlmMesh* AlmRenderComponent::GetMesh()
	{
		return m_mesh;
	}

	IAlmRenderMaterial* AlmRenderComponent::GetMaterial()
	{
		return m_material;
	}


	void AlmRenderComponent::SetName()
	{
		m_name = "AlmRenderComponent";
	}
}