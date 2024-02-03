#include <SDL.h>
#include "engine/platform/Logger.h"
#include "engine/platform/Window.h"
#include "engine/render/Renderer.h"
#include "engine/render/Drawer.h"
#include "engine/App.h"
#include <functional>
namespace Core
{

//TODO(Nacho): Remove globals
const float g_maxFPS = 60.0;
const float g_maxFPSms = 1000.0 / g_maxFPS;

App::App(AppContext i_appContext)
	: m_context(i_appContext)
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
		switch (e.type)
		{
			case SDL_QUIT:
			{
				m_running = false;
			}
			break;

			case SDL_KEYDOWN:
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_LEFT:
					case SDLK_a:
					{
						if(m_context.inputFNs.fLeftArrowPressed)
						{
							m_context.inputFNs.fLeftArrowPressed(*this);
						}
					}
					break;
					case SDLK_RIGHT:
					case SDLK_d:
					{
						if(m_context.inputFNs.fRightArrowPressed)
						{
							m_context.inputFNs.fRightArrowPressed(*this);
						}
					}
					break;
					case SDLK_SPACE:
					{
						if(m_context.inputFNs.fSpaceBarPressed)
						{
							m_context.inputFNs.fSpaceBarPressed(*this);
						}
					}
					break;
				}
			}
			break;

			case SDL_KEYUP:
			{
				case SDLK_LEFT:
				case SDLK_a:
				{
					if(m_context.inputFNs.fLeftArrowReleased)
					{
						m_context.inputFNs.fLeftArrowReleased(*this);
					}
				}
				break;
				case SDLK_RIGHT:
				case SDLK_d:
				{
					if(m_context.inputFNs.fRightArrowReleased)
					{
						m_context.inputFNs.fRightArrowReleased(*this);
					}
				}
				break;
			}
			break;
		}
	}
}

void App::Run()
{
	auto renderCb = std::function<void()>([this]
	{
		Render();
	});

	while(m_running)
	{
		static uint64_t start = SDL_GetPerformanceCounter();
		static float delatTime = g_maxFPSms;
		ProcessInput();
		Update(delatTime);
		m_renderer->Render(renderCb);

		static uint64_t end = SDL_GetPerformanceCounter();
		delatTime = (end - start)/(float)SDL_GetPerformanceFrequency() * 1000;

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
	m_window = new Window(m_context.appName, m_context.winDimX, m_context.winDimY);
	m_window->Init();

	m_renderer = new Renderer(m_window->GetSDLWindow());
	m_renderer->Init();

	m_drawer = new Drawer(m_renderer);
	// Init Renderer

	return true;
}

///////////////////////////////////////////////////////////////////////////////

void App::ShutDownServices()
{
	m_renderer->Shutdown();
	m_window->Shutdown();

	SDL_Quit();

	delete m_drawer;
	delete m_renderer;
	delete m_window;
}

} // namespace Core

