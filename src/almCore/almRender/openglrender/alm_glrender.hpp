#ifndef _ALM_OPENGL_RENDER_HPP_
#define _ALM_OPENGL_RENDER_HPP_

#include "../src/almCore/almRender/interface/alm_irendersys.hpp"

namespace alme
{
class AlmGLRender : public IAlmRenderSystem
{
public:
	~AlmGLRender();

	void InitRenderAPIInstance() override;
	void OnWindowResize(unsigned int width, unsigned int height) override;

	void BeginRender() override;
	void FinishRender() override;

	uint32_t GetTotalVideoMemmory() const override;
	uint32_t GetAvailableVideoMemmory() const override;
	std::string GetGpuVendorName() const override;

	IAlmRenderMaterial *CreateMaterial() override;

};

}
#endif // !_ALM_OPENGL_RENDER_HPP_
