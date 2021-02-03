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
		pass->SetVS("../../InfluxEngine/Resources/Shaders/DefaultVertexShader.hlsl", "VS_Main", Shader::Profile::PF_5_1);

		// Create the rootSignature object:
		pass->mpRootSignature = RootSignature::Create(device);

		// Create the pipeline object (using the root signature):
		pass->SetupPipelineStateObject(device);
		
		return pass;
	}

	// [TODO] Render to target FrameBuffer object:
	void RasterPass::Execute(sPtr<FrameBuffer> target)
	{
		auto device = LocateApp::Get()->GetDevice();
		auto cmdQueue = LocateApp::Get()->GetQueueManager()->GetQueue(QueueManager::QType::Direct);
		auto cmdList = cmdQueue->GetCommandList(device);

		// 0: Resource Binding & pipeline state:
		// [TODO] How do we approach the graphics root signature for these passes?
		ID3D12DescriptorHeap* ppHeaps[] = { nullptr, nullptr };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps); // can incur a pipeline flush (to be researched)
		cmdList->SetGraphicsRootSignature(mpRootSignature->GetDxRootSignature().Get()); // What type of resources are to be bound to the pipeline?
		SetupPipelineStateObject(device); // Creates a new PSO if the previous one was changed
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
	}

	bool RasterPass::SetVS(sPtr<Shader> vertexShader)
	{
		mpVertexShader = vertexShader;
		mStateChanged = true;
		return mpVertexShader.get();
	}
	bool RasterPass::SetVS(const std::string& fName, const std::string& entry, Shader::Profile shaderProfile)
	{
		return SetVS(sPtr<Shader>(Shader::Load(fName, Shader::Desc(entry, Shader::Type::VS, shaderProfile))));
	}

	// [TODO] Create a PSO that doesnt throw...
	void RasterPass::SetupPipelineStateObject(comPtr<ID3D12Device> device)
	{
		if (!mStateChanged) return;
		
		PipelineState::PipelineStateStream stateStream;

		stateStream.VS = CD3DX12_SHADER_BYTECODE(mpVertexShader->GetDxCompileBlob());
		//stateStream.PS = CD3DX12_SHADER_BYTECODE(mpPixelShader->GetDxCompileBlob());
		stateStream.pRootSignature = CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE(mpRootSignature->GetDxRootSignature().Get());
		stateStream.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		// [TODO] Get Primitive topologytype & vertexlayout from mesh data... (seperate struct)

		mpPipelineState = PipelineState::CreatePSO(device, mpRootSignature, stateStream);
		mStateChanged = false;
	}
}

