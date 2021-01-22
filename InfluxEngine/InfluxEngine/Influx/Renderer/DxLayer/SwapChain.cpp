#include "pch.h"
#include "SwapChain.h"
#include "DxLayer.h"

namespace Influx
{
	sPtr<SwapChain> SwapChain::Create(const Desc& desc, comPtr<ID3D12CommandQueue> commandQueue, comPtr<ID3D12Device2> device)
	{
		sPtr<SwapChain> swapChain(new SwapChain());

		// Create Swap Chain:
		swapChain->mpDxSwapChain = DxLayer::CreateSwapChain(desc.windowHandle, commandQueue.Get(), desc.dimensions.x, desc.dimensions.y, desc.bufferCount);
		swapChain->mCurBufferIdx = swapChain->mpDxSwapChain->GetCurrentBackBufferIndex();

		// Create buffers:
		swapChain->mpBuffers.resize(desc.bufferCount);
		swapChain->mRtDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		swapChain->mpRtDescHeap = DxLayer::CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, desc.bufferCount);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtDescHandle(swapChain->mpRtDescHeap->GetCPUDescriptorHandleForHeapStart());
		for (uint32_t i = 0; i < desc.bufferCount; ++i)
		{
			// Get the swapchain buffer:
			Ptr<ID3D12Resource> framebuffer;
			ThrowOnFail(swapChain->mpDxSwapChain->GetBuffer(i, IID_PPV_ARGS(&framebuffer)));

			device->CreateRenderTargetView(framebuffer, nullptr, rtDescHandle);
			swapChain->mpBuffers[i] = framebuffer;

			// Set offset for next location on the descriptor heap
			rtDescHandle.Offset(swapChain->mRtDescSize);
		}

		return swapChain;
	}

	Ptr<SwapChain::DxBuffer> SwapChain::GetCurrentBuffer() const
	{
		return GetBuffer(mpDxSwapChain->GetCurrentBackBufferIndex());
	}
	Ptr<SwapChain::DxBuffer> SwapChain::GetBuffer(uint32_t idx) const
	{
		assert(idx < mpBuffers.size());
		return mpBuffers[idx];
	}

	SwapChain::DxDescriptorHandle SwapChain::GetCurrentRtDescHandle() const
	{
		return GetDescRtHandle(mpDxSwapChain->GetCurrentBackBufferIndex());
	}
	SwapChain::DxDescriptorHandle SwapChain::GetDescRtHandle(uint32_t idx) const
	{
		assert(idx < mpBuffers.size());
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(mpRtDescHeap->GetCPUDescriptorHandleForHeapStart(), idx, mRtDescSize);
	}

	Influx::SwapChain::~SwapChain()
	{
		mpDxSwapChain->Release();
	}

	void SwapChain::Present(UINT syncintv, UINT presentFlags)
	{
		ThrowOnFail(mpDxSwapChain->Present(syncintv, presentFlags));
	}

	comPtr<IDXGISwapChain4> SwapChain::GetDxSwapChain() const
	{
		return mpDxSwapChain;
	}
}

