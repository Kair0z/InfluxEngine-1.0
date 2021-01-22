#include "pch.h"
#include "Renderer.h"

#include "DxLayer/SwapChain.h"
#include "DxLayer/CommandQueue.h"
#include "DxLayer/PipelineState.h"

#include "Gui/Gui.h"

namespace Influx
{
	sPtr<Renderer> Influx::Renderer::Create(const Desc& desc)
	{
		sPtr<Renderer> renderer(new Renderer());
		renderer->mConstructDesc = desc;
		return renderer;
	}

	void Renderer::Initialize()
	{
		if (mIsInitialized) return;

		// Enable the Debug layer
#ifdef _DEBUG
		DxLayer::EnableDebugLayer();
#endif

		// Load DxLayer Instance (Holds Adapter & Device)
		DxLayer::Desc dxDesc; dxDesc.mUseWarp = true;
		mpDx = sPtr<DxLayer>(DxLayer::LoadDX12(dxDesc));

		// Create Command Queue:
		CommandQueue::Desc queueDesc;
		queueDesc.type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		mpCommandQueue = CommandQueue::Create(mpDx->GetDevice(), queueDesc);

		// Create Swap Chain: 
		SwapChain::Desc swapDesc;
		swapDesc.bufferCount = 3;
		swapDesc.dimensions = mConstructDesc.dimensions;
		swapDesc.windowHandle = mConstructDesc.windowHandle;
		mpSwapChain = SwapChain::Create(swapDesc, mpCommandQueue->GetDxCommandQueue(), mpDx->GetDevice());

		// Create Pipeline State Object:
		mpPSO = PipelineState::Create(mpDx->GetDevice());
		
		// Initialize Gui:
		Gui::Dx12Impl_Desc guiDesc;
		guiDesc.bufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		guiDesc.nBuffers = swapDesc.bufferCount;
		guiDesc.windowHandle = mConstructDesc.windowHandle;

		Gui::Initialize(guiDesc, mpDx->GetDevice().Get());

		mIsInitialized = true;
	}

	void Renderer::Render()
	{
		// Render Gui (Internal)
		Gui::Render();

		ID3D12GraphicsCommandList2* cmdList = mpCommandQueue->GetCommandList(mpDx->GetDevice()); // Get a fresh command list from queue
		ID3D12Resource* backbuffer = mpSwapChain->GetCurrentBuffer(); // Get Swapchain Backbuffer

		// COMMAND: Transition backbuffer from a Present state into a Render Target state
		DxLayer::Cmd_TransitionResource(cmdList, backbuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		// COMMAND: Clear the rtv of the backbuffer
		Cmd_ClearRt(cmdList, Vector4f{ 0.4f, 0.6f, 0.9f, 1.0f });

		// COMMAND: Set render target to current backbuffer:
		Cmd_TargetBackbuffer(cmdList);

		// COMMAND: Render ImGui DrawData
		Gui::Cmd_SubmitDrawData(cmdList);

		// COMMAND: Transition backbuffer from a Render Target state back into a Present state
		DxLayer::Cmd_TransitionResource(cmdList, backbuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

		// EXECUTE commandlist:
		mpCommandQueue->ExecuteCommandList(cmdList);

		// Present frame:
		Present();
	}

	void Renderer::Cmd_ClearRt(Ptr<ID3D12GraphicsCommandList2> cmdList, const Vector4f& clearColor)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtDesc = mpSwapChain->GetCurrentRtDescHandle();
		cmdList->ClearRenderTargetView(rtDesc, &clearColor.x, 0, nullptr);
	}

	void Renderer::Cmd_TargetBackbuffer(Ptr<ID3D12GraphicsCommandList2> cmdList)
	{
		auto rtvHandle = mpSwapChain->GetCurrentRtDescHandle();
		cmdList->OMSetRenderTargets(1, &rtvHandle,FALSE, NULL);
	}

	void Renderer::Present()
	{
		mpSwapChain->Present(
			mConstructDesc.vSync ? 1 : 0,
			mConstructDesc.tearingSupported && !mConstructDesc.vSync ? DXGI_PRESENT_ALLOW_TEARING : 0);
	}
	void Renderer::Shutdown()
	{
		// Release all resources:
		//...

		// Check for live objects: 
		// (TODO: remove device from this report-list as it's obviously not yet released)
		DxLayer::ReportLiveObjects(mpDx->GetDevice().Get());

		// Release Device happens in smartpointer destructor...

		// Cleanup Gui:
		Gui::Cleanup();
	}
}

