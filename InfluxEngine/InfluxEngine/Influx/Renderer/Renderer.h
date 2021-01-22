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

		// Clear the Render Target View of the current Swapchainbuffer (backbuffer)
		void ClearRt(Ptr<ID3D12GraphicsCommandList2> cmdList, const Vector4f& clearColor);
		void Present();

		bool mIsInitialized = false;
		void Shutdown();
	};
}


