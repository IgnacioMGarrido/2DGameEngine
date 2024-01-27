#include <SDL.h>
#include "engine/platform/Logger.h"
#include "engine/platform/Window.h"
#include "engine/render/Renderer.h"
#include "engine/App.h"

namespace Core
{

//TODO(Nacho): Remove globals
const float g_maxFPS = 60.0;
const float g_maxFPSms = 1000.0 / g_maxFPS;

App::App(std::string i_appName)
	: m_appName(i_appName)
{
	InitServices();
}

///////////////////////////////////////////////////////////////////////////////

App::~App()
{
	ShutDownServices();
}

///////////////////////////////////////////////////////////////////////////////

void App::ProcessInput()
{
	SDL_Event e;
	while(SDL_PollEvent(&e) != 0)
	{
		if(e.type == SDL_QUIT)
		{
			m_running = false;
		}
	}

}

void App::Run()
{
	while(m_running)
	{
		static uint64_t start = SDL_GetPerformanceCounter();

		ProcessInput();
		Update(g_maxFPS);
		m_renderer->Render();

		static uint64_t end = SDL_GetPerformanceCounter();
		static float delatTime = (end - start)/(float)SDL_GetPerformanceFrequency() * 1000;

		if(delatTime < g_maxFPSms)
		{
			SDL_Delay(g_maxFPSms - delatTime);
		}

	}
}

///////////////////////////////////////////////////////////////////////////////

bool App::InitServices()
{

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_ERROR("Failed to Initialize SDL");
		return false;
	}

	// Init Window
	m_window = new Window(m_appName);
	m_window->Init();

	m_renderer = new Renderer(m_window->GetSDLWindow());
	m_renderer->Init();
	// Init Renderer

	return true;
}

///////////////////////////////////////////////////////////////////////////////

void App::ShutDownServices()
{
	m_renderer->Shutdown();
	m_window->Shutdown();

	SDL_Quit();
	
	delete m_renderer;
	delete m_window;
}

} // namespace Core

