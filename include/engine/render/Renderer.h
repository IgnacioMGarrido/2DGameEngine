#pragma once
#include "engine/Engine.h"
#include "engine/platform/IService.h"

struct SDL_Renderer;
struct SDL_Window;
namespace Core
{

class Renderer : public IService
{
public:
	Renderer(SDL_Window* i_window);
	virtual bool Init() override;
	virtual void Shutdown() override;

	void Render();

private:
	SDL_Renderer* m_renderer = nullptr;
	SDL_Window* m_targetWindow = nullptr;
};

} // namespace Core

