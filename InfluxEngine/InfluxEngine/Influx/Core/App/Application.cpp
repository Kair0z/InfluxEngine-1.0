#include "pch.h"
#include "Application.h"

#include "Influx/Core/Window/Window.h"
#include "Influx/Graphics/Renderer.h"
#include "Influx/Graphics/DxLayer/CommandQueue.h"
#include "Influx/Graphics/DxLayer/SwapChain.h"
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
			Update();

			// main render loop
			Render();
		}
	}

	comPtr<ID3D12Device> Application::GetDevice() const
	{
		return mpDx->GetDevice();
	}

	sPtr<QueueManager> Application::GetQueueManager() const
	{
		return mpQueueManager;
	}

	Application::AppInfo Application::GetAppInfo() const
	{
		AppInfo info;
		info.windowDimensions = mpWindow->GetWindowsDesc().dimensions;
		info.windowName = mpWindow->GetWindowsDesc().name;
		return info;
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
		uint8_t bufferCount = 3;
		mpWindow->SetupSwapChain(mpDx->GetDevice(), 
			mpQueueManager->GetQueue(QueueManager::QType::Direct)->GetDxCommandQueue(), bufferCount);

		// Initialize Gui:
		Gui::Dx12Impl_Desc guiDesc;
		guiDesc.bufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		guiDesc.nBuffers = bufferCount;
		guiDesc.windowHandle = mpWindow->GetWindowsHandle();
		Gui::Initialize(guiDesc, mpDx->GetDevice().Get());

		Gui::ListenOnRender(std::bind(&Application::OnGuiRender, this));
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

	void Application::Render()
	{
		Gui::Render();
		
		// Get the Direct Command Queue & a fresh command list:
		sPtr<CommandQueue> directCmdQueue = mpQueueManager->GetQueue(QueueManager::QType::Direct);
		ID3D12GraphicsCommandList2* cmdList = directCmdQueue->GetCommandList(mpDx->GetDevice());

		// Get Backbuffer from SwapChain: (And transition it to the Rt State)
		ID3D12Resource* backbuffer = mpWindow->GetSwapChain()->GetCurrentBuffer(); // Get Swapchain Backbuffer
		DxLayer::TransitionResource(cmdList, backbuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		
		auto bbCpuHandle = mpWindow->GetSwapChain()->GetCurrentRtDescHandle();

		// Clear backbuffer:
		const Vector4f clearColor{ 0.5f, 0.5f, 0.5f, 1 };
		cmdList->ClearRenderTargetView(bbCpuHandle, &clearColor.x, 0, nullptr);
		cmdList->OMSetRenderTargets(1, &bbCpuHandle, FALSE, nullptr);

		OnFrame();

		// Submit UI Draw Data:
		Gui::SubmitDrawData(cmdList);

		DxLayer::TransitionResource(cmdList, backbuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

		uint64_t f = directCmdQueue->ExecuteCommandList(cmdList);
		directCmdQueue->WaitForFence(f);

		mpWindow->GetSwapChain()->GetDxSwapChain()->Present(1, 0);
	}
}