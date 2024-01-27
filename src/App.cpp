#include <SDL.h>
#include "App.h"
#include "Logger.h"

namespace Core
{

//TODO(Nacho): Remove globals

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
int g_width = 1280;
int g_height = 720;
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

void App::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	SDL_RenderClear(g_renderer);
	SDL_RenderPresent(g_renderer);
}

void App::Run()
{
	while(m_running)
	{
		static uint64_t start = SDL_GetPerformanceCounter();

		ProcessInput();
		Update();
		Render();

		static uint64_t end = SDL_GetPerformanceCounter();
		static float delatTime = (end - start)/(float)SDL_GetPerformanceFrequency() * 1000;

		if(delatTime < g_maxFPS)
		{
			SDL_Delay(g_maxFPS - delatTime);
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

	g_window = SDL_CreateWindow(m_appName.c_str(),
								SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_width,
								g_height, SDL_WINDOW_SHOWN);
	if(!g_window)
	{
		LOG_ERROR("Failed to Initialize Window");
		return false;
	}

	// Init Renderer

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

	if(!g_renderer)
	{
		LOG_ERROR("Failed to Initialize Window");
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////

void App::ShutDownServices()
{
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	SDL_Quit();
}

} // namespace Core

