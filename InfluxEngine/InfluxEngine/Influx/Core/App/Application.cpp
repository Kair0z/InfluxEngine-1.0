#include "pch.h"
#include "Application.h"

#include "Influx/Core/Window/Window.h"
#include <iostream>

namespace Influx
{
	void Application::Run(HINSTANCE i)
	{
		Initialize(i);

		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			// Peek Windows Messages
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			// Update
			Update();
		}
	}

	void Application::Initialize(HINSTANCE i)
	{
		// Create the window
		Window::WindowDesc desc;
		desc.dimensions = {1280, 720};
		desc.name = L"Influx Demo";

		mpWindow = Window::Create(desc, i);

		// Show the window:
		::ShowWindow(mpWindow->GetWindowsHandle(), SW_SHOW);
	}

	void Application::Update()
	{
	}
}