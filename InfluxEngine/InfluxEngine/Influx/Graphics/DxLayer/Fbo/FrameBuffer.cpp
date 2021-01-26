#include "pch.h"
#include "FrameBuffer.h"
#include "Influx/Graphics/DxLayer/DxLayer.h"

namespace Influx
{
	sPtr<FrameBuffer> Influx::FrameBuffer::Create(comPtr<ID3D12Device> device, const Desc& desc)
	{
		sPtr<FrameBuffer> framebuffer(new FrameBuffer());

		framebuffer->mConstructDesc = desc;

		// Creating the color buffers:
		framebuffer->mpBuffers.resize(desc.nColorTargets);
		auto descheapType = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		framebuffer->mRtDescSize = device->GetDescriptorHandleIncrementSize(descheapType); // TODO: Must this always be RTV?
		framebuffer->mpRtDescHeap = DxLayer::CreateDescriptorHeap(device.Get(), descheapType, desc.nColorTargets);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtDescHandle(framebuffer->mpRtDescHeap->GetCPUDescriptorHandleForHeapStart());
		for (uint32_t i = 0; i < desc.nColorTargets; ++i)
		{
			// Get buffers[i]
			Ptr<ID3D12Resource> buffer;
			// [TODO]: Dx CREATING THE BUFFERS...

			//device->CreateRenderTargetView(buffer, nullptr, rtDescHandle);
			//framebuffer->mpBuffers[i] = buffer;

			// Offset next location:
			rtDescHandle.Offset(framebuffer->mRtDescSize);
		}

		// Creating depth buffer:
		framebuffer->mpDsvDescHeap = DxLayer::CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
		D3D12_CLEAR_VALUE optClearValue{};
		optClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		optClearValue.DepthStencil = { 1.0f, 0 };

		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto texDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, desc.dimensions.x, desc.dimensions.y,
			1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		ThrowOnFail(device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&texDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&optClearValue,
			IID_PPV_ARGS(&framebuffer->mpDepthBuffer)
			));

		// Create the Dsv
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		device->CreateDepthStencilView(framebuffer->mpDepthBuffer.Get(), &dsvDesc, framebuffer->mpDsvDescHeap->GetCPUDescriptorHandleForHeapStart());

		return framebuffer;
	}

	FrameBuffer::~FrameBuffer()
	{
		
	}

	void FrameBuffer::ClearColorTarget(Ptr<ID3D12GraphicsCommandList> cmdList, size_t colorTargetIdx, const Vector4f& clearColor)
	{
		assert(colorTargetIdx < mpBuffers.size());

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtDesc(mpRtDescHeap->GetCPUDescriptorHandleForHeapStart(), colorTargetIdx, mRtDescSize);
		cmdList->ClearRenderTargetView(rtDesc, &clearColor.r, 0, nullptr);
	}
	void FrameBuffer::ClearDepth(Ptr<ID3D12GraphicsCommandList> cmdList, const float depth)
	{
		auto dsv = mpDsvDescHeap->GetCPUDescriptorHandleForHeapStart();
		cmdList->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, depth, 0, 0, nullptr);
	}

	uint8_t FrameBuffer::GetNumColorTargets() const
	{
		return mConstructDesc.nColorTargets;
	}
	comPtr<FrameBuffer::DxBuffer> FrameBuffer::GetColorTarget(size_t idx) const
	{
		assert(idx < GetNumColorTargets());

		return mpBuffers[idx];
	}
	comPtr<FrameBuffer::DxBuffer> FrameBuffer::GetDepthTarget() const
	{
		return mpDepthBuffer;
	}
}

