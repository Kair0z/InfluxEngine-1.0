#include "pch.h"
#include "RasterPass.h"
#include "Influx/Graphics/DxLayer/PipelineState.h"
#include "Influx/Graphics/DxLayer/CommandQueue.h"
#include "Influx/Graphics/DxLayer/Fbo/FrameBuffer.h"
#include "Influx/Core/App/QueueManager.h"
#include "Influx/Core/App/Application.h"

namespace Influx
{
	sPtr<RasterPass> RasterPass::Create()
	{
		auto device = LocateApp::Get()->GetDevice();

		sPtr<RasterPass> pass(new RasterPass());

		// Create the pipeline object:
		PipelineState::Desc psoDesc{};
		pass->mpPipelineState = PipelineState::Create(device, psoDesc);

		
		
		return pass;
	}

	void RasterPass::Execute(sPtr<FrameBuffer> target)
	{
		auto device = LocateApp::Get()->GetDevice();
		auto cmdQueue = LocateApp::Get()->GetQueueManager()->GetQueue(QueueManager::QType::Direct);
		auto cmdList = cmdQueue->GetCommandList(device);

		/*cmdList->OMSetRenderTargets(1, &target->GetColorDescHandle(0), false,
			(target->HasDepthTarget() ? &target->GetDepthDescHandle() : nullptr) );*/
	}
}

