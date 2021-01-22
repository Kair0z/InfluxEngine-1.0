#include "pch.h"
#include "PipelineState.h"
#include "DxLayer.h"

namespace Influx
{
	sPtr<PipelineState> PipelineState::Create(comPtr<ID3D12Device2> device)
	{
		sPtr<PipelineState> pipelineState(new PipelineState());

		// TODO: static Create in DxLayer
		pipelineState->mpDxPipelineStateObject;

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

