#pragma once
#include "../Pass.h"

namespace Influx
{
	class FrameBuffer;
	class Scene;

	class PipelineState;

	class RasterPass final : public Pass
	{
	public:
		static sPtr<RasterPass> Create();
		void Execute(sPtr<FrameBuffer> target);

	private:
		sPtr<PipelineState> mpPipelineState;
		comPtr<ID3D12RootSignature> mpRootSignature;

		RasterPass() = default;
	};
}


