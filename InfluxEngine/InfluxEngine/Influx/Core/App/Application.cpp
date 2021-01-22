#include "pch.h"
#include "Application.h"

#include "Influx/Core/Window/Window.h"
#include "Influx/Graphics/Renderer.h"

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

			// Update:
			Update();

			// Render:
			mpRenderer->Render();
		}
	}

	void Application::Initialize(HINSTANCE i)
	{
		Vector2u dimensions = { 1280, 720 };
		// Create the window:
		Window::Desc desc;
		desc.dimensions = dimensions;
		desc.name = L"Influx Demo";

		mpWindow = Window::Create(desc, i);

		// Show the window:
		mpWindow->Show();

		// Create the renderer:
		Renderer::Desc renderDesc;
		renderDesc.dimensions = dimensions;
		renderDesc.vSync = true;
		renderDesc.tearingSupported = false;
		renderDesc.windowHandle = mpWindow->GetWindowsHandle();

		mpRenderer = Renderer::Create(renderDesc);
		mpRenderer->Initialize();
	}

	void Application::Update()
	{
	}

	sPtr<Renderer> Application::GetRenderer() const
	{
		return mpRenderer;
	}
}