#pragma once
#include "engine/Engine.h"
#include "engine/platform/IService.h"
#include <functional>
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
	SDL_Renderer* GetRendererPtr() const;
	void Render(std::function<void()>& i_renderCb);

private:
	SDL_Renderer* m_renderer = nullptr;
	SDL_Window* m_targetWindow = nullptr;
};

} // namespace Core

