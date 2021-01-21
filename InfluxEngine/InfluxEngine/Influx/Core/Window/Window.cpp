#include "pch.h"
#include "Window.h"

// Windows Callback function:
LRESULT CALLBACK WndProc(HWND hwnd, UINT mssg, WPARAM wParam, LPARAM lParam)
{
	switch (mssg)
	{
	case WM_SYSKEYDOWN:
		break;

	case WM_KEYDOWN:
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProcW(hwnd, mssg, wParam, lParam);
	}

	return 0;
}

namespace Influx
{
	sPtr<Window> Window::Create(const WindowDesc& wDesc, HINSTANCE hInst)
	{
		sPtr<Window> pWindow = sPtr<Window>(new Window());

		// Register window class:
		const wchar_t* wClass = L"DX12WindowClass";
		RegisterWindowClass(hInst, wClass);

		// Create Windows window:
		pWindow->mWindowHandle = CreateWindow(hInst, wDesc.name, wClass, wDesc.dimensions.x, wDesc.dimensions.y);

		return pWindow;
	}

	HWND Window::GetWindowsHandle() const
	{
		return mWindowHandle;
	}

	void Window::RegisterWindowClass(HINSTANCE i, const wchar_t* wndClassName)
	{
		WNDCLASSEXW windowClass = {};
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = &WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = i;
		windowClass.hIcon = ::LoadIcon(i, NULL);
		windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = wndClassName;
		windowClass.hIconSm = ::LoadIcon(i, NULL);

		static ATOM atom = ::RegisterClassExW(&windowClass);
		assert(atom > 0);
	}

	HWND Window::CreateWindow(HINSTANCE i, const wchar_t* title, const wchar_t* wndClassName, uint32_t w, uint32_t h)
	{
		// Setup Window Rect
		int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

		RECT wndRect = { 0, 0, (LONG)w, (LONG)h };
		::AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);

		int windowWidth = wndRect.right - wndRect.left;
		int windowHeight = wndRect.bottom - wndRect.top;
		int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
		int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

		// Create Window:
		HWND hWnd = ::CreateWindowExW(
			NULL,
			wndClassName,
			title,
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			i,
			nullptr
		);

		assert(hWnd && "Failed to Create Window...");

		return hWnd;
	}
}
