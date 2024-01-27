#pragma once
#include <string>

namespace Core
{
class Window;
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
	void Render();

protected:
	virtual void Update() = 0;
private:
	Window* m_window;
	bool m_running = true;
	std::string m_appName;
};

} // namespace Core

extern Core::App* CreateApp();
