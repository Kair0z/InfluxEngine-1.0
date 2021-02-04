#include "pch.h"
#include "RasterPass.h"
#include "Influx/Graphics/DxLayer/PipelineState.h"
#include "Influx/Graphics/DxLayer/CommandQueue.h"
#include "Influx/Graphics/DxLayer/Fbo/FrameBuffer.h"
#include "Influx/Graphics/DxLayer/RootSignature/RootSignature.h"
#include "Influx/Core/App/QueueManager.h"
#include "Influx/Core/App/Application.h"

namespace Influx
{
	sPtr<RasterPass> RasterPass::Create()
	{
		auto device = LocateApp::Get()->GetDevice();

		sPtr<RasterPass> pass(new RasterPass());

		// Set Default VertexShader:
		pass->SetVS("../InfluxEngine/Resources/Shaders/DefaultVertexShader.hlsl", "VS_Main", Shader::Profile::PF_5_1);

		// Create the rootSignature object:
		// [TODO: adding root parameters]
		std::vector<RootSignature::RootParameter> rootParams(1);
		rootParams[0].InitAsConstants(sizeof(Matrix4x4), 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);

		pass->mpRootSignature = RootSignature::Create(device, rootParams);

		return pass;
	}

	// [TODO] Render to target FrameBuffer object:
	void RasterPass::Execute(sPtr<FrameBuffer> target)
	{
		auto device = LocateApp::Get()->GetDevice();
		auto cmdQueue = LocateApp::Get()->GetQueueManager()->GetQueue(QueueManager::QType::Direct);
		auto cmdList = cmdQueue->GetCommandList(device);

		// 0: Resource Binding & pipeline state:
		// 0.a: Setting up the Root Signature:
		ID3D12DescriptorHeap* ppHeaps[] = { nullptr, nullptr };
		//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps); // can incur a pipeline flush (to be researched)
		cmdList->SetGraphicsRootSignature(mpRootSignature->GetDxRootSignature().Get()); // What type of resources are to be bound to the pipeline?

		// 0.b: Setting up pipelinestate:
		PipelineState::PipelineStateStream stateStream;
		//stateStream.PS = CD3DX12_SHADER_BYTECODE(mpPixelShader->GetDxCompileBlob());
		stateStream.VS = CD3DX12_SHADER_BYTECODE(mpVertexShader->GetDxCompileBlob());
		stateStream.pRootSignature = CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE(mpRootSignature->GetDxRootSignature().Get());
		stateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		stateStream.InputLayout = { inputLayout, _countof(inputLayout) };
		stateStream.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		stateStream.RTVFormats = rtvFormats;
		mpPipelineState = PipelineState::CreatePSO(device, mpRootSignature, stateStream);
		cmdList->SetPipelineState(mpPipelineState.Get());
		
		// 1: Configure Input Assembly [based on Scenedata]
		// cmdList->IASetIndexBuffer(); 
		// cmdList->IASetVertexBuffers();
		// cmdList->IASetPrimitiveTopology();

		// 2: RASTER STATE: [Based on Rasterpass settings]
		// cmdList->RSSetScissorRects();
		// Setup viewport
		D3D12_VIEWPORT vp{};
		vp.Width = (float)target->GetDimensions().x;
		vp.Height = (float)target->GetDimensions().y;
		vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
		vp.TopLeftX = vp.TopLeftY = 0.0f;
		cmdList->RSSetViewports(1, &vp);
		
		// 3: OUTPUT MERGER: [Based on Rasterpass settings]
		// cmdList->OMSetDepthBounds(); [REV]
		// cmdList->OMSetStencilRef(); [REV]
		// OM: Setup blend factor
		const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
		cmdList->OMSetBlendFactor(blend_factor);

		// OM: Setup Render Targets:
		auto rtvHandle = target->GetColorCpuHandle(0, FrameBuffer::ViewType::RTV);
		if (target->HasDepthTarget())
		{
			auto dsvHandle = target->GetDepthCpuHandle();
			cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
		}
		else cmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		auto fence = cmdQueue->ExecuteCommandList(cmdList);
		cmdQueue->WaitForFence(fence);
	}

	bool RasterPass::SetVS(sPtr<Shader> vertexShader)
	{
		mpVertexShader = vertexShader;
		return mpVertexShader.get();
	}
	bool RasterPass::SetVS(const std::string& fName, const std::string& entry, Shader::Profile shaderProfile)
	{
		return SetVS(sPtr<Shader>(Shader::Load(fName, Shader::Desc(entry, Shader::Type::VS, shaderProfile))));
	}
	void RasterPass::SetScene(sPtr<Scene> scene)
	{
		mpTargetScene = scene;
	}
}

