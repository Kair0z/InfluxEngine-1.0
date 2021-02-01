#include "pch.h"
#include "FrameBuffer.h"
#include "Influx/Graphics/DxLayer/DxLayer.h"
#include "Influx/Core/App/Application.h"
namespace Influx
{
	sPtr<FrameBuffer> FrameBuffer::Create(const Desc& desc)
	{
		return Create(LocateApp::Get()->GetDevice(), desc);
	}
	sPtr<FrameBuffer> Influx::FrameBuffer::Create(comPtr<ID3D12Device> device, const Desc& desc)
	{
		sPtr<FrameBuffer> framebuffer(new FrameBuffer());

		framebuffer->mConstructDesc = desc;
		framebuffer->mpBuffers.resize(desc.nColorTargets);

		// Creating the color buffers:
		// Rtv heap:
		framebuffer->mRtvDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		framebuffer->mpRtvDescHeap = DxLayer::CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, desc.nColorTargets);

		// Srv heap:
		framebuffer->mSrvDescSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		framebuffer->mpSrvDescHeap = DxLayer::CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, desc.nColorTargets);
		
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtDescHandle(framebuffer->mpRtvDescHeap->GetCPUDescriptorHandleForHeapStart());
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvDescHandle(framebuffer->mpSrvDescHeap->GetCPUDescriptorHandleForHeapStart());
		for (uint32_t i = 0; i < desc.nColorTargets; ++i)
		{
			// Creating the resource
			Ptr<ID3D12Resource> texture;
			DxLayer::Tex2DDesc texDesc;
			texDesc.dimensions = desc.dimensions;
			DxLayer::CreateResourceTexture2D(device.Get(), texDesc, &texture);
			framebuffer->mpBuffers[i] = texture;

			// Creating the Rtv: (Writing)
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
			rtvDesc.Format = texDesc.format;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			device->CreateRenderTargetView(texture, &rtvDesc, rtDescHandle);
			
			// Creating the Srv: (Reading)
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = texDesc.format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			device->CreateShaderResourceView(texture, &srvDesc, srvDescHandle);

			// Offset next location:
			rtDescHandle.Offset(framebuffer->mRtvDescSize);
			srvDescHandle.Offset(framebuffer->mSrvDescSize);
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

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtDesc(mpRtvDescHeap->GetCPUDescriptorHandleForHeapStart(), colorTargetIdx, mRtvDescSize);
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
	bool FrameBuffer::HasDepthTarget() const
	{
		return mConstructDesc.useDepthTarget;
	}
	comPtr<FrameBuffer::DxBuffer> FrameBuffer::GetColorTargetResource(size_t idx) const
	{
		assert(idx < GetNumColorTargets());
		return mpBuffers[idx];
	}
	comPtr<FrameBuffer::DxBuffer> FrameBuffer::GetDepthTargetResource() const
	{
		return mpDepthBuffer;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE FrameBuffer::GetColorCpuHandle(size_t idx, ViewType asView) const
	{
		assert(idx < GetNumColorTargets());
		switch (asView)
		{
		case ViewType::RTV: return CD3DX12_CPU_DESCRIPTOR_HANDLE(mpRtvDescHeap->GetCPUDescriptorHandleForHeapStart(), idx, mRtvDescSize);
		case ViewType::SRV: return CD3DX12_CPU_DESCRIPTOR_HANDLE(mpSrvDescHeap->GetCPUDescriptorHandleForHeapStart(), idx, mSrvDescSize);
		default: return CD3DX12_CPU_DESCRIPTOR_HANDLE(mpRtvDescHeap->GetCPUDescriptorHandleForHeapStart(), idx, mRtvDescSize);
		}
	}
	D3D12_GPU_DESCRIPTOR_HANDLE FrameBuffer::GetColorGpuHandle(size_t idx, ViewType asView) const
	{
		assert(idx < GetNumColorTargets());
		switch (asView)
		{
		case ViewType::RTV: return CD3DX12_GPU_DESCRIPTOR_HANDLE(mpRtvDescHeap->GetGPUDescriptorHandleForHeapStart(), idx, mRtvDescSize);
		case ViewType::SRV: return CD3DX12_GPU_DESCRIPTOR_HANDLE(mpSrvDescHeap->GetGPUDescriptorHandleForHeapStart(), idx, mSrvDescSize);
		default: return CD3DX12_GPU_DESCRIPTOR_HANDLE(mpRtvDescHeap->GetGPUDescriptorHandleForHeapStart(), idx, mRtvDescSize);
		}
	}

	D3D12_CPU_DESCRIPTOR_HANDLE FrameBuffer::GetDepthCpuHandle() const
	{
		assert(mConstructDesc.useDepthTarget);
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(mpDsvDescHeap->GetCPUDescriptorHandleForHeapStart());
	}
	D3D12_GPU_DESCRIPTOR_HANDLE FrameBuffer::GetDepthGpuHandle() const
	{
		assert(mConstructDesc.useDepthTarget);
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(mpDsvDescHeap->GetGPUDescriptorHandleForHeapStart());
	}
}

