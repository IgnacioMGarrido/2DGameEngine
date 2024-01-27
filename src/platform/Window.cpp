#include <SDL.h>
#include "engine/Engine.h"
#include "engine/platform/Window.h"
namespace Core
{

Window::Window(std::string i_appName, int i_width, int i_height)
	: m_appName(i_appName)
	, m_width(i_width)
	, m_height(i_height)
{

}

///////////////////////////////////////////////////////////////////////////////

SDL_Window* Window::GetSDLWindow() const
{
	return m_window;
}

///////////////////////////////////////////////////////////////////////////////

bool Window::Init()
{

	m_window = SDL_CreateWindow(m_appName.c_str(),
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width,
								m_height, SDL_WINDOW_SHOWN);
	if(!m_window)
	{
		LOG_ERROR("Failed to Initialize Window");
		return false;
	}

	return true;

}

///////////////////////////////////////////////////////////////////////////////

void Window::Shutdown()
{
	SDL_DestroyWindow(m_window);
}

} //namespace Core

