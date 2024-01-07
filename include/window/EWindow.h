#pragma once
#include <Windows.h>

namespace core::platform
{
class EWindow
{
public:
	EWindow(int i_width, int i_height, const char* i_name);

	void UpdateWindowMessages();

private:
	bool Init(int i_width, int i_height, const char* i_name);

private:
	int m_width;
	int m_height;
	const char* m_name;
	HWND m_hwnd;
};
} //namespace core
