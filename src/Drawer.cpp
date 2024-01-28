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

void Drawer::DrawFilledRect(const Rect& i_rectangle, const Color& i_color)
{
	SDL_FRect sdlRect = { .x = i_rectangle.x, .y = i_rectangle.y, .w = i_rectangle.w, .h = i_rectangle.h };

	SDL_SetRenderDrawColor(m_renderer->GetRendererPtr(), i_color.r, i_color.g, i_color.b, 255);
	SDL_RenderFillRectF(m_renderer->GetRendererPtr(), &sdlRect);
}

}
