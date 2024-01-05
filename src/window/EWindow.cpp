#include "window/EWindow.h"
#include <Logger.h>
namespace core::platform
{

namespace
{

//TODO(Nacho): This is a global for now we will set this elsewhere
static bool s_running;

LRESULT CALLBACK WinWindowsCb(HWND i_wnd, UINT i_msg, WPARAM i_wParam, LPARAM i_lParam)
{
	LRESULT result = 0;
	switch(i_msg)
	{
		case WM_SIZE:
		{
			LOG_TRACE("WM_SIZE");
		}
		break;
		case WM_CLOSE:
		{
			//TODO(Nacho): Handle with a message
			s_running = false;
			LOG_TRACE("WM_CLOSE");
		}
		break;
		case WM_ACTIVATEAPP:
		{
			LOG_TRACE("WM_ACTIVATEAPP");
		}
		break;
		case WM_DESTROY:
		{
			//TODO(Nacho): Handle with an error
			s_running = false;
			LOG_TRACE("WM_DESTROY");
		}
		break;
		case WM_PAINT:
		{
			LOG_TRACE("WM_PAINT");
			PAINTSTRUCT paint;
			HDC deviceContext = BeginPaint(i_wnd, &paint);
			int x = paint.rcPaint.left;
			int y = paint.rcPaint.top;
			int width = paint.rcPaint.right - paint.rcPaint.left;
			int height = paint.rcPaint.bottom - paint.rcPaint.top;

			static DWORD operation = WHITENESS;
			PatBlt(deviceContext, x, y, height, width, operation);

			if(operation == WHITENESS)
			{
				operation = BLACKNESS;
			}
			else
			{
				operation = WHITENESS;
			}
			EndPaint(i_wnd, &paint);
		}
		break;
		default:
		{
			result = DefWindowProcA(i_wnd, i_msg, i_wParam, i_lParam);
		}
	}

	return result;
}
}


EWindow::EWindow(const WinData& i_winData)
{
	if(!Init(i_winData))
	{
		LOG_ERROR("Failed to Initialize Window");
	}
}

///////////////////////////////////////////////////////////////////////////////

bool EWindow::Init(const WinData& i_winData)
{
	HINSTANCE instance = GetModuleHandleA(0);

	WNDCLASSA wc = {};
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = i_winData.name;
	wc.lpfnWndProc = WinWindowsCb;

	if (!RegisterClassA(&wc))
	{
		LOG_ERROR("Window: Cannot register window Class");
		return false;
	}

	m_hwnd = CreateWindowExA(
		0,
		i_winData.name,
		i_winData.name,
		WS_OVERLAPPEDWINDOW,
		100, 100, i_winData.width, i_winData.height,
		NULL,
		NULL,
		instance,
		NULL
	);
	if (m_hwnd == nullptr)
	{
		LOG_ERROR("Window: Cannot Create  Window.");
		return false;
	}
	//TODO(im): get the long ptr for the window
	ShowWindow(m_hwnd, SW_SHOW);
	return true;
}

///////////////////////////////////////////////////////////////////////////////

void EWindow::UpdateWindow()
{
	s_running = true;
	while(s_running)
	{
		MSG message;
		BOOL messageResult = GetMessageA(&message, m_hwnd, 0, 0);
		if(messageResult > 0)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

} //namespace core
