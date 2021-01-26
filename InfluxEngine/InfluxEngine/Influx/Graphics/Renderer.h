#pragma once
#include "Influx/Core/Alias.h"
#include "DxLayer/DxLayer.h"
#include "Influx/Resources/Mesh/CubeMesh.h"

namespace Influx
{
	class SwapChain;
	class CommandQueue;
	class PipelineState;
	class IndexBuffer;
	class VertexBuffer;

	class Renderer final
	{
	public:
		struct Desc
		{
			HWND windowHandle;
			Vector2u dimensions;

			D3D12_VIEWPORT mViewport;
			D3D12_RECT mScissorRect;

			bool tearingSupported;
			bool vSync;
		};
		static sPtr<Renderer> Create(const Desc& desc);
		IFX_DelCpyMove(Renderer);

		void Initialize();
		void Render();

		~Renderer() { Shutdown(); }

	private:
		Renderer() = default;

		void LoadContent();

		Desc mConstructDesc;

		// Dx-Related Stuff
		
		sPtr<SwapChain> mpSwapChain; // Swap Chain:

		comPtr<ID3D12RootSignature> mpRootSignature;
		sPtr<CommandQueue> mpCommandQueue; // CommandQueue:
		sPtr<PipelineState> mpPSO; // Pipeline State Object:

		sPtr<VertexBuffer> mpVertexBuffer_Temp;
		sPtr<IndexBuffer> mpIndexBuffer_Temp;

		comPtr<ID3D12Resource> mpDxDepthBuffer;
		comPtr<ID3D12DescriptorHeap> mpDSVHeap;

		comPtr<ID3DBlob> mpVShaderBlob;
		comPtr<ID3DBlob> mpPShaderBlob;

		

		// Clear the Render Target View of the current Swapchainbuffer (backbuffer)
		void Cmd_ClearRt(Ptr<ID3D12GraphicsCommandList2> cmdList, const Vector4f& clearColor);
		void Cmd_ClearDepth(Ptr<ID3D12GraphicsCommandList2> cmdList, const float depth = 1.0f);

		void Cmd_TargetBackbuffer(Ptr<ID3D12GraphicsCommandList2> cmdList);
		void Present();

		void CreateDepthBuffer();

		bool mIsInitialized = false;
		void Shutdown();
	};
}


