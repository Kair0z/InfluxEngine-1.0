#include "pch.h"
#include "Application.h"

#include "Influx/Core/Window/Window.h"
#include "Influx/Graphics/Renderer.h"
#include "Influx/Graphics/DxLayer/CommandQueue.h"
#include "Influx/Graphics/Gui/Gui.h"
#include "QueueManager.h"

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

		//When quit button is pressed
		while (msg.message != WM_QUIT)
		{
			// Peek Windows Messages
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			// Update:
			//Update();

			// [TODO] main render loop
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

		// Setup DxLayer:
		DxLayer::Desc dxDesc;
		dxDesc.mUseWarp = true;
		mpDx = DxLayer::LoadDX12(dxDesc);

		// Initialize our command queues:
		mpQueueManager = QueueManager::CreateQueues(mpDx->GetDevice());

		// Setup window Swapchain:
		mpWindow->SetupSwapChain(mpDx->GetDevice(), 
			mpQueueManager->GetQueue(QueueManager::QueueTag::Direct)->GetDxCommandQueue());
	}

	void Application::Update()
	{
		mTimer.Tick();

		//Temporary - FPS text counter [REV]
		std::wstring fps = mpWindow->GetWindowsDesc().name;
		fps += L" | FPS: ";
		fps += std::to_wstring(mTimer.GetFramesPerSecond());
		
		//Set windows top border text
		SetWindowText(mpWindow->GetWindowsHandle(), fps.c_str());
	}
}