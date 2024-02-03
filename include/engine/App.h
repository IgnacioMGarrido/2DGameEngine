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

	struct InputFns
	{
		void (*fSpaceBarPressed)(Core::App& myApp);
		void (*fRightArrowPressed)(Core::App& myApp);
		void (*fLeftArrowPressed)(Core::App& myApp);
		void (*fLeftArrowReleased)(Core::App& myApp);
		void (*fRightArrowReleased)(Core::App& myApp);
	};

	struct AppContext
	{
		std::string appName;
		int winDimX;
		int winDimY;
		InputFns inputFNs;
	};

	App(AppContext i_appContext);
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
	AppContext m_context;

};

} // namespace Core

extern Core::App* CreateApp();
