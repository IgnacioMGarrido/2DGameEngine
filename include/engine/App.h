#pragma once
#include <string>

namespace Core
{
class Window;
class Renderer;
class Drawer;
class App
{
public:

	App(std::string i_appName);
	~App();
	void Run();

private:
	bool InitServices();
	void ShutDownServices();
	void ProcessInput();

protected:
	virtual void Update(float i_fixedTick) = 0;
	virtual void Render() = 0;
protected:
	Drawer* m_drawer;

private:
	Window* m_window;
	Renderer* m_renderer;
	bool m_running = true;
	std::string m_appName;
};

} // namespace Core

extern Core::App* CreateApp();
