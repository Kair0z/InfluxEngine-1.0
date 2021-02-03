#pragma once

struct ID3D12Device2;
struct ID3D12PipelineState;

/// <summary>
/// Wrapper class for D3D12 Pipeline State Object
/// A PSO is immutable after creation!
/// https://developer.nvidia.com/dx12-dos-and-donts#pipeline
/// </summary>

namespace Influx
{
	struct Shader;
	class RootSignature;
	namespace PipelineState
	{
		struct PipelineStateStream final
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
			CD3DX12_PIPELINE_STATE_STREAM_VS VS;
			CD3DX12_PIPELINE_STATE_STREAM_PS PS;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
		};
		comPtr<ID3D12PipelineState> CreatePSO(comPtr<ID3D12Device> device, sPtr<RootSignature> rootSignature, PipelineStateStream statestream);
	}
}


