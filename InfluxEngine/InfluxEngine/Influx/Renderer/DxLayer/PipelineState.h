#pragma once

struct ID3D12Device2;
struct ID3D12PipelineStateObject;

/// <summary>
/// Wrapper class for D3D12 Pipeline State Object
/// </summary>

namespace Influx
{
	class PipelineState final
	{
	public:
		static sPtr<PipelineState> Create(sPtr<ID3D12Device2> device);
		~PipelineState();

		sPtr<ID3D12PipelineStateObject> GetDxPipelineStateObject() const;

	private:
		PipelineState() = default;
		IFX_DelCpyMove(PipelineState);

		sPtr<ID3D12PipelineStateObject> mpDxPipelineStateObject;
	};
}


