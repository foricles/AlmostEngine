#ifndef _ALM_WINDOW_INTERFACE_HPP_
#define _ALM_WINDOW_INTERFACE_HPP_

#include "../src/almCore/alm_engsystem.hpp"
#include "../src/almCore/almEvents/alm_delegat.hpp"

namespace alme
{

class IAlmWindow : public AlmEventHandler, protected AlmEngineSystem
{
public:
	enum WindowMode
	{
		eWM_Windowed,
		eWM_Fullscreen
	};

public:
	IAlmWindow(AlmostEngine *engine) 
		: AlmEngineSystem(engine) {}

	virtual void			Hide() = 0;
	virtual void			Show() = 0;
	virtual void			Maximize() = 0;

	virtual void			SetSize(uint32_t width, uint32_t height) = 0;
	virtual uint32_t		GetWidth() const = 0;
	virtual uint32_t		GetHeight() const = 0;

	virtual void			SetPos(uint32_t posX, uint32_t posY) = 0;
	virtual uint32_t		GetPosX() const = 0;
	virtual uint32_t		GetPosY() const = 0;

	virtual void			SetTitle(const std::string &title) = 0;
	virtual std::string 	GetTitle() const = 0;

	virtual void			Quit() = 0;
	virtual bool			WasWindowClosed() = 0;

	virtual void			SetWindowMode(WindowMode mode) = 0;

	virtual void			Update() = 0;

	AlmDelegate<>				refresh_callback;
	AlmDelegate<>				on_close;
	AlmDelegate<int>			maximize_callback;
	AlmDelegate<int>			focus_callback;
	AlmDelegate<int, int>		pos_callback;
	AlmDelegate<int, int>		size_callback;
	AlmDelegate<int, int>		canvas_size_callback;
};

}


#endif // !_ALM_WINDOW_INTERFACE_HPP_