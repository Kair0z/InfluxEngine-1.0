#include "pch.h"
#include "Application.h"

#include "Influx/Core/Window/Window.h"
#include "Influx/Graphics/Renderer.h"
#include "Influx/Graphics/Gui/Gui.h"

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
		renderDesc.mScissorRect = CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX);
		renderDesc.mViewport = CD3DX12_VIEWPORT(0.0f, 0.0f, float(dimensions.x), float(dimensions.y));
		mpRenderer = Renderer::Create(renderDesc);
		mpRenderer->Initialize();

		// Subscribe to Gui Callback:
		Gui::ListenOnRender(std::bind(&Application::OnGuiRender, this));
	}

	void Application::Update()
	{
	}

	sPtr<Renderer> Application::GetRenderer() const
	{
		return mpRenderer;
	}
}