#pragma once
#include "engine/platform/IService.h"
#include <string>
namespace Core
{

class Window : public IService
{
public:
	Window(std::string i_appName, int i_width = 1280, int i_height = 720);

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

