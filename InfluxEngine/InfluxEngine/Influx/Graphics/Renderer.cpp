#include "pch.h"
#include "Renderer.h"

#include "DxLayer/SwapChain.h"
#include "DxLayer/CommandQueue.h"
#include "DxLayer/PipelineState.h"
#include "DxLayer/IndexBuffer.h"
#include "DxLayer/VertexBuffer.h"

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

		// Create Depth buffer:
		// DSV descriptor heap
		CreateDepthBuffer();

		// Initialize Gui:
		Gui::Dx12Impl_Desc guiDesc;
		guiDesc.bufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		guiDesc.nBuffers = swapDesc.bufferCount;
		guiDesc.windowHandle = mConstructDesc.windowHandle;
		Gui::Initialize(guiDesc, mpDx->GetDevice().Get());

		// Load external content:
		LoadContent();

		mIsInitialized = true;
	}

	void Renderer::Render()
	{
		// Render Gui (Internal)
		Gui::Render();

		ID3D12GraphicsCommandList2* cmdList = mpCommandQueue->GetCommandList(mpDx->GetDevice()); // Get a fresh command list from queue
		ID3D12Resource* backbuffer = mpSwapChain->GetCurrentBuffer(); // Get Swapchain Backbuffer

		// COMMAND: backbuffer: Present --> RT
		DxLayer::Cmd_TransitionResource(cmdList, backbuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		// COMMAND: Clear the rtv of the backbuffer
		Cmd_ClearRt(cmdList, Vector4f{ 0.4f, 0.6f, 0.9f, 1.0f });
		Cmd_ClearDepth(cmdList, 1.0f);

		// COMMAND: Render ImGui DrawData
		Gui::SubmitDrawData(cmdList);

		// COMMAND: backbuffer: RT --> Present
		DxLayer::Cmd_TransitionResource(cmdList, backbuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

		// EXECUTE commandlist:
		uint64_t f = mpCommandQueue->ExecuteCommandList(cmdList);
		mpCommandQueue->WaitForFence(f);

		// Present frame:
		Present();
	}

	void Renderer::LoadContent()
	{
		// Setup a cpy command queue...
		auto device = mpDx->GetDevice().Get();
		CommandQueue::Desc queueDesc;
		queueDesc.type = D3D12_COMMAND_LIST_TYPE_COPY;
		auto cmdQueue = CommandQueue::Create(device, queueDesc);
		auto cmdList = cmdQueue->GetCommandList(device);

		// Upload buffers:
		mpVertexBuffer_Temp = VertexBuffer::Create();
		mpVertexBuffer_Temp->Upload<Vertex>(cmdList, device, Temp::gCubeVertices, _countof(Temp::gCubeVertices));
		mpIndexBuffer_Temp = IndexBuffer::Create();
		mpIndexBuffer_Temp->Upload(cmdList, device, Temp::gCubeIndicies, _countof(Temp::gCubeIndicies));

		// Load Shaders:
		auto vsFile = L"../../InfluxEngine/Resources/Shaders/DefaultVertexShader.hlsl";		  // SHOULD BE RE-STRUCTURED
		auto psFile = L"../../InfluxEngine/Resources/Shaders/DefaultPixelShader.hlsl";		  // SHOULD BE RE-STRUCTURED
																							  // SHOULD BE RE-STRUCTURED
		DxLayer::CompileShader(vsFile, "VS_Main", "vs_5_1", &mpVShaderBlob);				  // SHOULD BE RE-STRUCTURED
		DxLayer::CompileShader(psFile, "PS_Main", "ps_5_1", &mpPShaderBlob);				  // SHOULD BE RE-STRUCTURED

		// Create Root Signature:
		// Allow input layout and deny unnecessary access to certain pipeline stages.
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

		// A single 32-bit constant root parameter that is used by the vertex shader. (WVP matrix)
		CD3DX12_ROOT_PARAMETER1 rootParameters[1];
		rootParameters[0].InitAsConstants(sizeof(Matrix4x4), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

		auto rsDesc = DxLayer::CreateRootSignatureDesc(rootParameters, rootSignatureFlags);
		mpRootSignature = DxLayer::CreateRootSignature(mpDx->GetDevice().Get(), &rsDesc);

		// TODO: Create PipelineStateObject:
		// Define RTV formats
		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Define Input Layout:
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		PipelineState::Desc psoDesc;
		psoDesc.mStateStream.InputLayout = { inputLayout, _countof(inputLayout) };
		psoDesc.mStateStream.pRootSignature = mpRootSignature.Get();
		psoDesc.mStateStream.PS = CD3DX12_SHADER_BYTECODE(mpPShaderBlob.Get());
		psoDesc.mStateStream.VS = CD3DX12_SHADER_BYTECODE(mpVShaderBlob.Get());
		psoDesc.mStateStream.RTVFormats = rtvFormats;
		psoDesc.mStateStream.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psoDesc.mStateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		mpPSO = PipelineState::Create(mpDx->GetDevice(), psoDesc);

		auto fence = cmdQueue->ExecuteCommandList(cmdList);
		cmdQueue->WaitForFence(fence);
	}



	void Renderer::Cmd_TargetBackbuffer(Ptr<ID3D12GraphicsCommandList2> cmdList)
	{
		auto rtvHandle = mpSwapChain->GetCurrentRtDescHandle();
		auto dsvHandle = mpDSVHeap->GetCPUDescriptorHandleForHeapStart();
		cmdList->OMSetRenderTargets(1, &rtvHandle,FALSE, &dsvHandle);
	}

	void Renderer::Present()
	{
		mpSwapChain->Present(
			mConstructDesc.vSync ? 1 : 0,
			mConstructDesc.tearingSupported && !mConstructDesc.vSync ? DXGI_PRESENT_ALLOW_TEARING : 0);
	}
	void Renderer::Shutdown()
	{
		// Cleanup Gui:
		Gui::Cleanup();

		if (mpDxDepthBuffer) mpDxDepthBuffer->Release();
		if (mpRootSignature) mpRootSignature->Release();
		if (mpPShaderBlob) mpPShaderBlob->Release();
		if (mpVShaderBlob) mpVShaderBlob->Release();
		if (mpDSVHeap) mpDSVHeap->Release();
	}
}

