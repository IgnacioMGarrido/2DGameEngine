#include <SDL.h>
#include "engine/render/Drawer.h"
#include "engine/render/Renderer.h"
namespace Core
{

Drawer::Drawer(Renderer* i_renderer, int i_canvasWidth, int i_canvasHeight)
	: m_renderer(i_renderer)
	, m_canvasWidth(i_canvasWidth)
	, m_canvasHeight(i_canvasHeight)
{
}

void Drawer::DrawFilledRect(int i_xpos, int i_ypos, int i_width, int i_height, uint8_t i_r, uint8_t i_g ,uint8_t i_b)
{
	SDL_Rect green_rect_filled = { .x = i_xpos, .y = i_ypos, .w = i_width, .h = i_height };

	SDL_SetRenderDrawColor(m_renderer->GetRendererPtr(), i_r, i_g, i_b, 255);
	SDL_RenderFillRect(m_renderer->GetRendererPtr(), &green_rect_filled);
}

}
