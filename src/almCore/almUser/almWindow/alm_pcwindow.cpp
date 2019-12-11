#include "alm_pcwindow.hpp"
#include "../src/almCore/alm_platform.hpp"

using namespace alme;

static AlmWindowPC *globMainPc = nullptr;

#define NOT_NULL(x, m) if(x != nullptr){ x->m(); return 0;}

////platform specific/////
#ifdef ALM_OS_WINDOWS
HWND globHwnd;
WNDCLASS globWc;
HINSTANCE globHInstance;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CreateMainWindow()
{
	globHInstance = GetModuleHandle(0);
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	globWc.lpfnWndProc = WindowProc;
	globWc.hInstance = globHInstance;
	globWc.lpszClassName = CLASS_NAME;

	RegisterClass(&globWc);

	// Create the window.

	globHwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		globHInstance,  // Instance handle
		NULL        // Additional application data
	);

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY: NOT_NULL(globMainPc, Quit);
		case WM_SIZE: NOT_NULL(globMainPc, OnWindowResize);
		case WM_MOVE: NOT_NULL(globMainPc, OnWindowChangePos);
			
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#endif // 


//////////////////////////

AlmWindowPC::AlmWindowPC(AlmostEngine *engine)
	: IAlmWindow(engine)
{
	globMainPc = this;
	CreateMainWindow();
	Show();
}

AlmWindowPC::~AlmWindowPC()
{
	globMainPc = nullptr;
}

void AlmWindowPC::Hide()
{
}

void AlmWindowPC::Show()
{
	ShowWindow(globHwnd, SW_SHOW);
}

void AlmWindowPC::Maximize()
{
}

void AlmWindowPC::SetSize(uint32_t width, uint32_t height)
{
}

uint32_t AlmWindowPC::GetWidth() const
{
	return uint32_t();
}

uint32_t AlmWindowPC::GetHeight() const
{
	return uint32_t();
}

void AlmWindowPC::SetPos(uint32_t posX, uint32_t posY)
{
}

uint32_t AlmWindowPC::GetPosX() const
{
	return uint32_t();
}

uint32_t AlmWindowPC::GetPosY() const
{
	return uint32_t();
}

void AlmWindowPC::SetTitle(const std::string & title)
{
}

std::string AlmWindowPC::GetTitle() const
{
	return std::string();
}

void AlmWindowPC::Quit()
{
	DestroyWindow(globHwnd);
	on_close.Execute();
}

bool AlmWindowPC::WasWindowClosed()
{
	return false;
}

void AlmWindowPC::SetWindowMode(WindowMode mode)
{
}

void AlmWindowPC::Update()
{
#ifdef ALM_OS_WINDOWS
	MSG msg = { };
	if (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#endif
}

void AlmWindowPC::OnWindowResize()
{
#ifdef ALM_OS_WINDOWS
	RECT rect;
	if (GetWindowRect(globHwnd, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		size_callback.Execute(width, height);
	}
	if (GetClientRect(globHwnd, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		canvas_size_callback.Execute(width, height);
	}
#endif // ALM_OS_WINDOWS

}

void AlmWindowPC::OnWindowChangePos()
{
}
