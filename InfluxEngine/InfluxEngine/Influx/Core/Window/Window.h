#pragma once
#include <string>
#include "Influx/Core/Alias.h"
#include "Influx/Math/Math.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Influx
{
	struct Window
	{
	public:
		struct WindowDesc
		{
			const wchar_t* name;
			Vector2u dimensions;
		};
		static sPtr<Window> Create(const WindowDesc& wDesc, HINSTANCE hInst);

		HWND GetWindowsHandle() const;

	private:
		Window() = default;

		HWND mWindowHandle;

		static void RegisterWindowClass(HINSTANCE i, const wchar_t* wndClassName);
		static HWND CreateWindow(HINSTANCE i, const wchar_t* title, const wchar_t* wndClassName, uint32_t w, uint32_t h);
	};
}

