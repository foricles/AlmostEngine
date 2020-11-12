#ifndef _ALM_RENDER_COMPONENT_HPP_
#define _ALM_RENDER_COMPONENT_HPP_

#include "alm_icomponent.hpp"

namespace alme
{
	class IAlmMesh;
	class IAlmRenderMaterial;
	class AlmRenderComponent : public IAlmComponentImp<AlmRenderComponent>
	{
	public:
		AlmRenderComponent(AlmEntity* owner);
		~AlmRenderComponent();

		void					SetMesh(IAlmMesh* mesh);
		void					SetMaterial(IAlmRenderMaterial* material);
		IAlmMesh*				GetMesh();
		IAlmRenderMaterial*		GetMaterial();

	private:
		void					SetName() override;

	private:
		IAlmMesh*				m_mesh;
		IAlmRenderMaterial*		m_material;

	public:
		AlmRenderComponent(AlmRenderComponent&& rhv) = delete;
		AlmRenderComponent(const AlmRenderComponent& rhv) = delete;
		AlmRenderComponent& operator=(AlmRenderComponent&& rhv) = delete;
		AlmRenderComponent& operator=(const AlmRenderComponent& rhv) = delete;
	};
}

#endif // !_ALM_RENDER_COMPONENT_HPP_
