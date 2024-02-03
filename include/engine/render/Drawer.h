#pragma once

namespace Core
{
class Renderer;
class Drawer
{
public:

	struct Rect
	{
		float x;
		float y;
		float w;
		float h;
	};

	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	Drawer(Renderer* i_renderer);

	void DrawFilledRect(const Rect& i_rectangle, const Color& i_color);
private:
	Renderer* m_renderer;
};
}

