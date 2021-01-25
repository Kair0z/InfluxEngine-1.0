#pragma once
#include <string>
#include "Influx/Core/Alias.h"
#include "Influx/Math/Math.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

namespace Influx
{
	class SwapChain;
	struct Window
	{
	public:
		struct Desc
		{
			const wchar_t* name;
			Vector2u dimensions;
		};
		static sPtr<Window> Create(const Desc& wDesc, HINSTANCE hInst);
		IFX_DelCpyMove(Window);

		void Show();
		HWND GetWindowsHandle() const;

		static void SetWindowIcon(const std::string& iconPath, const HWND& windowHandle);

	private:
		Window() = default;

		HWND mWindowHandle;

		static void RegisterWindowClass(HINSTANCE i, const wchar_t* wndClassName);
		static HWND CreateWindow(HINSTANCE i, const wchar_t* title, const wchar_t* wndClassName, uint32_t w, uint32_t h);
	};
}


