#pragma once
#include "Influx/Core/Alias.h"
#include "DxLayer/DxLayer.h"

namespace Influx
{
	class SwapChain;
	class CommandQueue;
	class PipelineState;

	class Renderer final
	{
	public:
		struct Desc
		{
			HWND windowHandle;
			Vector2u dimensions;

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

		Desc mConstructDesc;

		// Dx-Related Stuff
		sPtr<DxLayer> mpDx;
		sPtr<SwapChain> mpSwapChain; // Swap Chain:
		sPtr<CommandQueue> mpCommandQueue; // CommandQueue:
		sPtr<PipelineState> mpPSO; // Pipeline State Object:

		comPtr<ID3D12Resource> mpDxDepthBuffer;
		comPtr<ID3D12DescriptorHeap> mpDSVHeap;

		// Clear the Render Target View of the current Swapchainbuffer (backbuffer)
		void Cmd_ClearRt(Ptr<ID3D12GraphicsCommandList2> cmdList, const Vector4f& clearColor);
		void Cmd_ClearDepth(Ptr<ID3D12GraphicsCommandList2> cmdList, const float depth = 1.0f);

		void Cmd_TargetBackbuffer(Ptr<ID3D12GraphicsCommandList2> cmdList);
		void Present();

		bool mIsInitialized = false;
		void Shutdown();
	};
}


