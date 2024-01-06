#include "window/EWindow.h"
#include <Logger.h>
namespace core::platform
{

namespace
{

//TODO(Nacho): This is a global for now we will set this elsewhere
static bool s_running;
static BITMAPINFO s_bitmapInfo;
static void* s_bitmapMemory;
static int s_bitmapWidth;
static int s_bitmapHeight;
static int s_bytesPerPixel = 4;

///////////////////////////////////////////////////////////////////////////////

static void DrawGradient(int i_xOffset, int i_yOffset)
{
	int pitch = s_bitmapWidth * s_bytesPerPixel;
	uint8_t* row = (uint8_t*) s_bitmapMemory;
	for(int y = 0; y < s_bitmapHeight; y++)
	{
		uint32_t* pixel = (uint32_t*) row;
		for(int x = 0; x < s_bitmapWidth; x++)
		{
			uint8_t b = x + i_xOffset;
			uint8_t r = y + i_yOffset;
			*pixel++ = (r << 16 | b);
		}

		row += pitch;
	}

}

static void ResizeDIBSection(int i_width, int i_height)
{
	s_bitmapWidth = i_width;
	s_bitmapHeight = i_height;

	s_bitmapInfo.bmiHeader.biSize = sizeof(s_bitmapInfo.bmiHeader);
	s_bitmapInfo.bmiHeader.biWidth = s_bitmapWidth;
	s_bitmapInfo.bmiHeader.biHeight = -s_bitmapHeight;
	s_bitmapInfo.bmiHeader.biPlanes = 1;
	s_bitmapInfo.bmiHeader.biBitCount = 32;
	s_bitmapInfo.bmiHeader.biCompression = BI_RGB;

	int bitmapMemorySize = s_bytesPerPixel * s_bitmapWidth * s_bitmapHeight;
	s_bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

	DrawGradient(5, 8);

}

///////////////////////////////////////////////////////////////////////////////

static void UpdateWindowBuffer(HDC i_deviceContext, RECT* i_rect, int i_x, int i_y, int i_width, int i_height)
{

	int windowWidth = i_rect->right - i_rect->left;
	int windowHeight = i_rect->bottom - i_rect->top;
	StretchDIBits(i_deviceContext,
				  0, 0, s_bitmapWidth, s_bitmapHeight,
				  0, 0, windowWidth, windowHeight,
				  s_bitmapMemory, &s_bitmapInfo,
				  DIB_RGB_COLORS, SRCCOPY);
}

///////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WinWindowsCb(HWND i_wnd, UINT i_msg, WPARAM i_wParam, LPARAM i_lParam)
{
	LRESULT result = 0;
	switch(i_msg)
	{
		case WM_SIZE:
		{
			RECT clientRect;
			GetClientRect(i_wnd, &clientRect);
			int width = clientRect.right - clientRect.left;
			int height = clientRect.bottom - clientRect.top;

			ResizeDIBSection(width, height);
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

			RECT clientRect;
			GetClientRect(i_wnd, &clientRect);
			UpdateWindowBuffer(deviceContext, &clientRect, x, y, width, height);
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
	LOG_ASSERT(Init(i_winData), "Failed to Initialize Window");
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
	ShowWindow(m_hwnd, SW_SHOW);
	return true;
}

///////////////////////////////////////////////////////////////////////////////

void EWindow::UpdateWindowMessages()
{
	s_running = true;
	int xOffset = 0;
	int yOffset = 0;

	while(s_running)
	{
		MSG message;
		while(PeekMessage(&message, m_hwnd, 0, 0, PM_REMOVE))
		{
			if(message.message == WM_QUIT)
			{
				s_running = false;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		DrawGradient(xOffset, yOffset);
		++xOffset;
		++yOffset;

		RECT clientRect;
		GetClientRect(m_hwnd, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;
		HDC deviceContext = GetDC(m_hwnd);
		UpdateWindowBuffer(deviceContext,
						   &clientRect,
						   0,
						   0,
						   width,
						   height);
		ReleaseDC(m_hwnd, deviceContext);
	}
}

///////////////////////////////////////////////////////////////////////////////

} //namespace core
