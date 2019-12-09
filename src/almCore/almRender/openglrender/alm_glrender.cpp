#include "alm_glrender.hpp"
#include "../src/almCore/alm_log.hpp"
#include "../src/almCore/alm_platform.hpp"
#include <glew/glew.h>

using namespace alme;

#ifdef ALM_OS_WINDOWS

HDC hDC;				/* device context */
HGLRC hRC;				/* opengl context */

#endif // ALM_OS_WINDOWS

AlmGLRender::~AlmGLRender()
{
#ifdef ALM_OS_WINDOWS
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(GetActiveWindow(), hDC);
#endif // ALM_OS_WINDOWS
}

void AlmGLRender::InitRenderAPIInstance()
{
#ifdef ALM_OS_WINDOWS

	hDC = GetDC(GetActiveWindow());

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  /* size */
		1,                              /* version */
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               /* support double-buffering */
		PFD_TYPE_RGBA,                  /* color type */
		16,                             /* prefered color depth */
		0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
		0,                              /* no alpha buffer */
		0,                              /* alpha bits (ignored) */
		0,                              /* no accumulation buffer */
		0, 0, 0, 0,                     /* accum bits (ignored) */
		16,                             /* depth buffer */
		0,                              /* no stencil buffer */
		0,                              /* no auxiliary buffers */
		PFD_MAIN_PLANE,                 /* main layer */
		0,                              /* reserved */
		0, 0, 0,                        /* no layer, visible, damage masks */
	};

	int pixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (pixelFormat == 0) ALM_LOG_ERROR("ChoosePixelFormat failed!");
	if (SetPixelFormat(hDC, pixelFormat, &pfd) != TRUE) ALM_LOG_ERROR("SetPixelFormat failed!");

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);

#endif // ALM_OS_WINDOWS

	glewInit();
}


void AlmGLRender::OnWindowResize(unsigned int width, unsigned int height)
{
	glViewport(0, 0, width, height);
}

void AlmGLRender::BeginRender()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AlmGLRender::FinishRender()
{
#ifdef ALM_OS_WINDOWS
	SwapBuffers(hDC);
#endif // ALM_OS_WINDOWS
}

IAlmRenderMaterial * AlmGLRender::CreateMaterial()
{
	return nullptr;
}
