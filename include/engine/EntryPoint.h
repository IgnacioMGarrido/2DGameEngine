#pragma once
#include "App.h"

int main(int argc, char** argv)
{
	Core::App* app = CreateApp();

	app->Run();

	delete app;

}
