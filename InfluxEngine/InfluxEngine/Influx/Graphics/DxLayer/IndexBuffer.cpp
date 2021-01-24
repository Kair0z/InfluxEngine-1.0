#include "pch.h"
#include "IndexBuffer.h"
#include "DxLayer.h"

namespace Influx
{
	sPtr<IndexBuffer> IndexBuffer::Create()
	{
		sPtr<IndexBuffer> idxBuffer(new IndexBuffer());
		return idxBuffer;
	}

	void IndexBuffer::Upload(Ptr<ID3D12GraphicsCommandList> cmdList, comPtr<ID3D12Device> device, Index const* pData, uint32_t n)
	{
		UINT eSize = sizeof(pData[0]);
		UINT eCount = n;
		
		DxLayer::LoadBufferResource(device.Get(), cmdList, &mpDxIndexBuffer, &mpDxInterBuffer,
			eCount, eSize, (void*)pData, D3D12_RESOURCE_FLAG_NONE);

		// Create the IndexbufferView:
		mIndexBufferView.BufferLocation = mpDxIndexBuffer->GetGPUVirtualAddress();
		mIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
		mIndexBufferView.SizeInBytes = eSize * eCount;
	}

	comPtr<ID3D12Resource> IndexBuffer::GetDxIndexBuffer() const
	{
		return mpDxIndexBuffer;
	}

	IndexBuffer::~IndexBuffer()
	{
	}
}

