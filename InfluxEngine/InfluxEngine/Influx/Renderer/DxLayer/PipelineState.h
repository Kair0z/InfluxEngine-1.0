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
	public:
		static sPtr<PipelineState> Create(comPtr<ID3D12Device2> device);
		~PipelineState();

		comPtr<ID3D12PipelineState> GetDxPipelineStateObject() const;

	private:
		PipelineState() = default;
		IFX_DelCpyMove(PipelineState);

		comPtr<ID3D12PipelineState> mpDxPipelineStateObject;
	};
}


