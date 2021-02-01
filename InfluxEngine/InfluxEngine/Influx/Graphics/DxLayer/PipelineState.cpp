#include "pch.h"
#include "PipelineState.h"
#include "DxLayer.h"

namespace Influx
{
	sPtr<PipelineState> PipelineState::Create(comPtr<ID3D12Device> device, const Desc& desc)
	{
		sPtr<PipelineState> pipelineState(new PipelineState());
		pipelineState->mStream = desc.mStateStream;

		D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
			sizeof(PipelineStateStream), &pipelineState->mStream
		};
		pipelineState->mpDxPipelineStateObject = DxLayer::CreatePipelineState(device.Get(), pipelineStateStreamDesc);
		return pipelineState;
	}

	PipelineState::~PipelineState()
	{
		if (mpDxPipelineStateObject)
			mpDxPipelineStateObject->Release();
	}

	comPtr<ID3D12PipelineState> PipelineState::GetDxPipelineStateObject() const
	{
		return mpDxPipelineStateObject;
	}
}

