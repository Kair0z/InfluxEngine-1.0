#pragma once

struct ID3D12Device2;
struct IDXGISwapChain4;

namespace Influx
{
	class SwapChain final
	{
		using DxBuffer = ID3D12Resource;
		using DxDescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE;

	public:
		struct Desc
		{
			HWND windowHandle;
			Vector2u dimensions;
			uint32_t bufferCount;
		};
		static sPtr<SwapChain> Create(const Desc& desc, comPtr<ID3D12CommandQueue> commandQueue, comPtr<ID3D12Device2> device);

		~SwapChain();

		void Present(UINT syncintv, UINT presentFlags);
		
		comPtr<IDXGISwapChain4> GetDxSwapChain() const;

		Ptr<DxBuffer> GetCurrentBuffer() const;
		Ptr<DxBuffer> GetBuffer(uint32_t idx) const;

		DxDescriptorHandle GetCurrentRtDescHandle() const;
		DxDescriptorHandle GetDescRtHandle(uint32_t idx) const;

	private:
		SwapChain() = default;

		comPtr<IDXGISwapChain4> mpDxSwapChain;
		comPtr<ID3D12DescriptorHeap> mpRtDescHeap; // The descriptor heap 4 our render targets
		std::vector<Ptr<DxBuffer>> mpBuffers;

		uint32_t mCurBufferIdx;
		uint32_t mRtDescSize;
	};
}


