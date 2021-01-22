#include "pch.h"
#include "PipelineState.h"
#include "DxLayer.h"

namespace Influx
{
	sPtr<PipelineState> PipelineState::Create(sPtr<ID3D12Device2> device)
	{
		sPtr<PipelineState> pipelineState(new PipelineState());

		// TODO: static Create in DxLayer
		pipelineState->mpDxPipelineStateObject;

		return pipelineState;
	}

	PipelineState::~PipelineState()
	{
		// TODO: Release Pipeline State object
	}

	sPtr<ID3D12PipelineStateObject> PipelineState::GetDxPipelineStateObject() const
	{
		return mpDxPipelineStateObject;
	}
}

