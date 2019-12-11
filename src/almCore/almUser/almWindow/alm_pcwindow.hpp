#ifndef _ALM_PC_WINDOW_HPP_
#define _ALM_PC_WINDOW_HPP_

#include "alm_iwindow.hpp"

namespace alme
{

class AlmWindowPC : public IAlmWindow
{
public:
	AlmWindowPC(AlmostEngine *engine);
	~AlmWindowPC();

	void			Hide() override;
	void			Show() override;
	void			Maximize() override;

	void			SetSize(uint32_t width, uint32_t height) override;
	uint32_t		GetWidth() const override;
	uint32_t		GetHeight() const override;

	void			SetPos(uint32_t posX, uint32_t posY) override;
	uint32_t		GetPosX() const override;
	uint32_t		GetPosY() const override;

	void			SetTitle(const std::string &title) override;
	std::string 	GetTitle() const override;

	void			Quit() override;
	bool			WasWindowClosed() override;
	void			SetWindowMode(WindowMode mode) override;

	void			Update() override;

	void OnWindowResize();
	void OnWindowChangePos();
};

}

#endif // !_ALM_PC_WINDOW_HPP_