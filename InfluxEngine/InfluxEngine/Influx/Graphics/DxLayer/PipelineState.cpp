#include "pch.h"
#include "PipelineState.h"
#include "DxLayer.h"
#include "Influx/Graphics/DxLayer/RootSignature/RootSignature.h"

namespace Influx
{
	namespace PipelineState
	{
		comPtr<ID3D12PipelineState> CreatePSO(comPtr<ID3D12Device> device, sPtr<RootSignature> rootSignature, PipelineStateStream statestream)
		{
			statestream.pRootSignature = CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE(rootSignature->GetDxRootSignature().Get());

			D3D12_PIPELINE_STATE_STREAM_DESC pipelineStateStreamDesc = {
				sizeof(PipelineStateStream), &statestream
			};

			return DxLayer::CreatePipelineState(device.Get(), pipelineStateStreamDesc);
		}
	}
}


