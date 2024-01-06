#pragma once
#include <Windows.h>

namespace core::platform
{
class EWindow
{
public:
	struct WinData
	{
		int width;
		int height;
		const char* name;
	};
	EWindow(const WinData& i_winData);

	void UpdateWindowMessages();

private:
	bool Init(const WinData& i_winData);

private:
	HWND m_hwnd;
};
} //namespace core
