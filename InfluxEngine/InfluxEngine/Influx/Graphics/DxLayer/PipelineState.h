#pragma once

struct ID3D12Device2;
struct ID3D12PipelineState;

/// <summary>
/// Wrapper class for D3D12 Pipeline State Object
/// </summary>

namespace Influx
{
	class PipelineState final
	{
		struct PipelineStateStream
		{
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
			CD3DX12_PIPELINE_STATE_STREAM_VS VS;
			CD3DX12_PIPELINE_STATE_STREAM_PS PS;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
		};

	public:
		struct Desc
		{
			PipelineStateStream mStateStream;

			// Additional Settings:
		};
		static sPtr<PipelineState> Create(comPtr<ID3D12Device> device, const Desc& desc);
		~PipelineState();

		comPtr<ID3D12PipelineState> GetDxPipelineStateObject() const;

	private:
		PipelineState() = default;
		IFX_DelCpyMove(PipelineState);

		PipelineStateStream mStream;
		comPtr<ID3D12PipelineState> mpDxPipelineStateObject;
	};
}


