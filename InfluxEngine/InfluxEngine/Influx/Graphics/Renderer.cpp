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

		cmdList->SetPipelineState(mpPSO->GetDxPipelineStateObject().Get());
		// We explicitly set the root signature before binding resources to the pipeline... (else runtime error)
		cmdList->SetGraphicsRootSignature(mpRootSignature.Get());

		// Set Input assembler:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		auto vtxBufferView = mpVertexBuffer_Temp->GetView();
		auto idxBufferView = mpIndexBuffer_Temp->GetView();
		cmdList->IASetIndexBuffer(&idxBufferView);
		cmdList->IASetVertexBuffers(0, 1, &vtxBufferView);

		// Set Viewport & ScissorRect
		cmdList->RSSetViewports(1, &mConstructDesc.mViewport);
		cmdList->RSSetScissorRects(1, &mConstructDesc.mScissorRect);

		// Set Output Merger: Binding the render target & DepthBuffer:
		Cmd_TargetBackbuffer(cmdList);

		// Update Root Params:
		Matrix4x4 w = glm::mat4x4();
		Matrix4x4 v = glm::lookAtLH(Vector3f{ 0, 0, -10 }, Vector3f{ 0, 0, 1 }, Vector3f{ 0, 1, 0 });
		Matrix4x4 p = glm::perspectiveFovLH_ZO(45.0f, (float)mConstructDesc.dimensions.x, (float)mConstructDesc.dimensions.y, 0.001f, 100000.0f);

		Matrix4x4 wvp = glm::transpose(w * v * p);
		cmdList->SetGraphicsRoot32BitConstants(0, sizeof(Matrix4x4), &w, 0);

		// Draw call
		cmdList->DrawIndexedInstanced(_countof(Temp::gCubeIndicies), 1, 0, 0, 0);

		// COMMAND: Render ImGui DrawData
		Gui::Cmd_SubmitDrawData(cmdList);

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
		auto vsFile = L"D:/Main/Git/InfluxEngine-1.0/InfluxEngine/InfluxEngine/Shaders/DefaultVertexShader.hlsl";
		auto psFile = L"D:/Main/Git/InfluxEngine-1.0/InfluxEngine/InfluxEngine/Shaders/DefaultPixelShader.hlsl";

		DxLayer::CompileShader(vsFile, "VS_Main", "vs_5_1", &mpVShaderBlob);
		DxLayer::CompileShader(psFile, "PS_Main", "ps_5_1", &mpPShaderBlob);

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

	void Renderer::CreateDepthBuffer()
	{
		mpDSVHeap = DxLayer::CreateDescriptorHeap(mpDx->GetDevice().Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
		D3D12_CLEAR_VALUE optimizedClearValue = {};
		optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		optimizedClearValue.DepthStencil = { 1.0f, 0 };

		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto texDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, mConstructDesc.dimensions.x, mConstructDesc.dimensions.y,
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		// Creating Depth Texture...
		ThrowOnFail(mpDx->GetDevice()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&texDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&optimizedClearValue,
			IID_PPV_ARGS(&mpDxDepthBuffer)
		));

		// Update the depth-stencil view, and store it in the DSV Descriptor heap... (at heap start of course)
		D3D12_DEPTH_STENCIL_VIEW_DESC dsv = {};
		dsv.Format = DXGI_FORMAT_D32_FLOAT;
		dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv.Texture2D.MipSlice = 0;
		dsv.Flags = D3D12_DSV_FLAG_NONE;

		mpDx->GetDevice()->CreateDepthStencilView(mpDxDepthBuffer.Get(), &dsv,
			mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void Renderer::Cmd_ClearRt(Ptr<ID3D12GraphicsCommandList2> cmdList, const Vector4f& clearColor)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtDesc = mpSwapChain->GetCurrentRtDescHandle();
		cmdList->ClearRenderTargetView(rtDesc, &clearColor.x, 0, nullptr);
	}

	void Renderer::Cmd_ClearDepth(Ptr<ID3D12GraphicsCommandList2> cmdList, const float depth)
	{
		auto dsv = mpDSVHeap->GetCPUDescriptorHandleForHeapStart();
		cmdList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, depth, 0, 0, nullptr);
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
	}
}

