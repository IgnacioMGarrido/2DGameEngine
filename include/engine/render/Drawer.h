#pragma once

namespace Core
{
class Renderer;
class Drawer
{
public:
	Drawer(Renderer* i_renderer, int i_canvasWidth, int i_canvasHeight);

	void DrawFilledRect(int i_xpos, int i_ypos, int i_width, int i_height, uint8_t i_r, uint8_t i_g ,uint8_t i_b);
private:
	int m_canvasWidth;
	int m_canvasHeight;
	Renderer* m_renderer;
};
}

