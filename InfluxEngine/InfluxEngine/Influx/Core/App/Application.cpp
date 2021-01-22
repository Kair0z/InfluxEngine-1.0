#include "pch.h"
#include "Application.h"

#include "Influx/Core/Window/Window.h"
#include "Influx/Renderer/Renderer.h"

namespace Influx
{
	void Application::Run(HINSTANCE i)
	{
		if (mState == AppState::Running) return;

		mState = AppState::Uninitialized;
		Initialize(i);

		// Call On Start after initialization
		OnStart();

		MSG msg = {};
		mState = AppState::Running;
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
		// Create the window:
		Window::Desc desc;
		desc.dimensions = {1280, 720};
		desc.name = L"Influx Demo";

		mpWindow = Window::Create(desc, i);

		// Show the window:
		mpWindow->Show();

		// Create the renderer:
		Renderer::Desc renderDesc;
		mpRen = Renderer::Create(renderDesc);
		mpRen->Initialize();
	}

	void Application::Update()
	{
	}

	sPtr<Renderer> Application::GetRenderer() const
	{
		return mpRen;
	}
}