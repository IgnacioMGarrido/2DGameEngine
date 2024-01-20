#include "window/EWindow.h"
#include <Logger.h>
namespace core::platform
{

struct OffscreenBuffer
{
	BITMAPINFO info;
	void* memory;
	int width;
	int height;
	int bytesPerPixel = 4;
};

///////////////////////////////////////////////////////////////////////////////

struct WindowDimensions
{
	int width;
	int height;
};

///////////////////////////////////////////////////////////////////////////////

//TODO(Nacho): Remove static global variables
static OffscreenBuffer s_offscreenBuffer;
static bool s_running;

///////////////////////////////////////////////////////////////////////////////

static WindowDimensions GetWindowDimensions(HWND i_wnd)
{
	WindowDimensions windowDimensions;
	RECT clientRect;
	GetClientRect(i_wnd, &clientRect);
	windowDimensions.width = clientRect.right - clientRect.left;
	windowDimensions.height = clientRect.bottom - clientRect.top;

	return windowDimensions;
}

///////////////////////////////////////////////////////////////////////////////

static void DrawGradient(OffscreenBuffer& i_offscreenBuffer, int i_xOffset, int i_yOffset)
{
	int pitch = i_offscreenBuffer.width * i_offscreenBuffer.bytesPerPixel;
	uint8_t* row = (uint8_t*) i_offscreenBuffer.memory;
	for(int y = 0; y < i_offscreenBuffer.height; y++)
	{
		uint32_t* pixel = (uint32_t*) row;
		for(int x = 0; x < i_offscreenBuffer.width; x++)
		{
			uint8_t b = x + i_xOffset;
			uint8_t r = y + i_yOffset;
			*pixel++ = (r << 16 | b);
		}

		row += pitch;
	}

}

///////////////////////////////////////////////////////////////////////////////

static void ResizeDIBSection(OffscreenBuffer& i_offscreenBuffer, int i_width, int i_height)
{
	i_offscreenBuffer.width = i_width;
	i_offscreenBuffer.height = i_height;

	i_offscreenBuffer.info.bmiHeader.biSize = sizeof(i_offscreenBuffer.info.bmiHeader);
	i_offscreenBuffer.info.bmiHeader.biWidth = i_offscreenBuffer.width;
	i_offscreenBuffer.info.bmiHeader.biHeight = -i_offscreenBuffer.height;
	i_offscreenBuffer.info.bmiHeader.biPlanes = 1;
	i_offscreenBuffer.info.bmiHeader.biBitCount = 32;
	i_offscreenBuffer.info.bmiHeader.biCompression = BI_RGB;

	int bitmapMemorySize = i_offscreenBuffer.bytesPerPixel * i_offscreenBuffer.width * i_offscreenBuffer.height;
	i_offscreenBuffer.memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

///////////////////////////////////////////////////////////////////////////////

static void UpdateWindowBuffer(HDC i_deviceContext, int i_windowWidth, int i_windowHeight, int i_x, int i_y, int i_width, int i_height)
{
	//TODO(Nacho): Find a way to maintain Aspect Ratio of the image.

	StretchDIBits(i_deviceContext,
				  0, 0, i_windowWidth, i_windowHeight,
				  0, 0, s_offscreenBuffer.width, s_offscreenBuffer.height,
				  s_offscreenBuffer.memory, &s_offscreenBuffer.info,
				  DIB_RGB_COLORS, SRCCOPY);
}

///////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WinWindowsCb(HWND i_wnd, UINT i_msg, WPARAM i_wParam, LPARAM i_lParam)
{
	//TODO(Nacho): Obtain the class Pointer to access the offsetBuffer.
	LRESULT result = 0;
	switch(i_msg)
	{
		case WM_SIZE:
		{
		}
		break;
		case WM_CLOSE:
		{
			//TODO(Nacho): Handle with a message
			s_running = false;
		}
		break;
		case WM_ACTIVATEAPP:
		{
		}
		break;
		case WM_DESTROY:
		{
			//TODO(Nacho): Handle with an error
			s_running = false;
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT paint;
			HDC deviceContext = BeginPaint(i_wnd, &paint);
			int x = paint.rcPaint.left;
			int y = paint.rcPaint.top;
			int width = paint.rcPaint.right - paint.rcPaint.left;
			int height = paint.rcPaint.bottom - paint.rcPaint.top;

			auto winDim = GetWindowDimensions(i_wnd);
			UpdateWindowBuffer(deviceContext, winDim.width, winDim.height, x, y, width, height);
		}
		break;
		default:
		{
			result = DefWindowProcA(i_wnd, i_msg, i_wParam, i_lParam);
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////


EWindow::EWindow(int i_width, int i_height, const char* i_name)
	: m_width(i_width)
	, m_height(i_height)
	, m_name(i_name)
{
	const bool initSuccess = Init(i_width, i_height, i_name);
	LOG_ASSERT(initSuccess, "Failed to Initialize Window");
}

///////////////////////////////////////////////////////////////////////////////

bool EWindow::Init(int i_width, int i_height, const char* i_name)
{
	HINSTANCE instance = GetModuleHandleA(0);
	ResizeDIBSection(s_offscreenBuffer, i_width, i_height);

	WNDCLASSA wc = {};
	wc.hInstance = instance;
	wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = i_name;
	wc.lpfnWndProc = WinWindowsCb;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassA(&wc))
	{
		LOG_ERROR("Window: Cannot register window Class");
		return false;
	}

	m_hwnd = CreateWindowExA(
		0,
		i_name,
		i_name,
		WS_OVERLAPPEDWINDOW,
		100, 100, i_width, i_height,
		nullptr,
		nullptr,
		instance,
		nullptr
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

		DrawGradient(s_offscreenBuffer, xOffset, yOffset);
		++xOffset;
		yOffset += 2;

		auto winDim = GetWindowDimensions(m_hwnd);
		HDC deviceContext = GetDC(m_hwnd);
		UpdateWindowBuffer(deviceContext,
						   winDim.width,
						   winDim.height,
						   0,
						   0,
						   winDim.width,
						   winDim.height);
		ReleaseDC(m_hwnd, deviceContext);
	}
}

///////////////////////////////////////////////////////////////////////////////

} //namespace core
