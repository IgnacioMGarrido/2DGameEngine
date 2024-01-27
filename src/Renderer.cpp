#include <SDL.h>
#include "engine/Engine.h"
#include "engine/render/Renderer.h"

namespace Core
{

Renderer::Renderer(SDL_Window* i_window)
	: m_targetWindow(i_window)
{
}

///////////////////////////////////////////////////////////////////////////////

bool Renderer::Init()
{
	LOG_ASSERT(m_targetWindow, "Renderer Target Window is null! something went really bad");

	m_renderer = SDL_CreateRenderer(m_targetWindow, -1, SDL_RENDERER_ACCELERATED);

	if(!m_renderer)
	{
		LOG_ERROR("Failed to Initialize SDL_Renderer");
		return false;
	}

	return true;

}

SDL_Renderer* Renderer::GetRendererPtr() const
{
	return m_renderer;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::Shutdown()
{
	SDL_DestroyRenderer(m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::Render(std::function<void()>& i_renderCb)
{
	LOG_ASSERT(m_targetWindow, "Renderer Target Window is null! something went really bad");
	SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	i_renderCb();

	SDL_RenderPresent(m_renderer);
}

///////////////////////////////////////////////////////////////////////////////


} // namespace Core

