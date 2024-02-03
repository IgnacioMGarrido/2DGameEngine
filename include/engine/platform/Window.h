#pragma once
#include "engine/Engine.h"
#include "engine/platform/IService.h"
#include <string>

struct SDL_Window;
namespace Core
{

class Window : public IService
{
public:
	Window(std::string i_appName, int i_width, int i_height);
	
	SDL_Window* GetSDLWindow() const;
	virtual bool Init() override;
	virtual void Shutdown() override;

private:
	SDL_Window* m_window = nullptr;
	int m_width;
	int m_height;

	std::string m_appName;
};
} // namespce core

