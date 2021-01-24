#pragma once

namespace Influx
{
	class VertexBuffer final
	{
	public:
		inline static sPtr<VertexBuffer> Create()
		{
			return sPtr<VertexBuffer>(new VertexBuffer());
		}

		// Send internal data to GPU
		template<typename VertexType>
		inline void Upload(Ptr<ID3D12GraphicsCommandList> cmdList, comPtr<ID3D12Device> device, VertexType const* pData, uint32_t n)
		{
			UINT eSize = sizeof(VertexType);
			UINT eCount = n;
			DxLayer::LoadBufferResource(device.Get(), cmdList, &mpDxVertexBuffer, &mpDxInterBuffer,
				eCount, eSize, (void*)pData, D3D12_RESOURCE_FLAG_NONE);

			// Create the VertexbufferView:
			mVertexBufferView.BufferLocation = mpDxVertexBuffer->GetGPUVirtualAddress();
			mVertexBufferView.SizeInBytes = eSize * eCount;
			mVertexBufferView.StrideInBytes = eSize;
		}

		inline D3D12_VERTEX_BUFFER_VIEW GetView() const { return mVertexBufferView; }
		inline comPtr<ID3D12Resource> GetDxVertexBuffer() const { return mpDxVertexBuffer; }

		inline ~VertexBuffer() {}

	private:
		VertexBuffer() = default;

		comPtr<ID3D12Resource> mpDxVertexBuffer;
		comPtr<ID3D12Resource> mpDxInterBuffer;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	};
}
