#pragma once
#include "../Pass.h"
#include "Influx/Graphics/DxLayer/Shaders/Shader.h"

struct ID3D12PipelineState;

namespace Influx
{
	class FrameBuffer;
	class Scene;	
	class RootSignature;

	class RasterPass final : public Pass
	{
	public:
		static sPtr<RasterPass> Create();

		void Execute(sPtr<FrameBuffer> target);

		bool SetVS(sPtr<Shader> vertexShader);
		bool SetVS(const std::string& fName, const std::string& entry, Shader::Profile shaderProfile);

		bool SetPS(sPtr<Shader> pixelShader);
		bool SetPS(const std::string& fName, const std::string& entry, Shader::Profile shaderProfile);

		void SetScene(sPtr<Scene> scene);

	private:
		comPtr<ID3D12PipelineState> mpPipelineState;
		sPtr<RootSignature> mpRootSignature;

		sPtr<Shader> mpVertexShader;
		sPtr<Shader> mpPixelShader;

		sPtr<Scene> mpTargetScene;

		RasterPass() = default;
	};
}


