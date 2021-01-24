#pragma once

struct ID3D12CommandList;
struct ID3D12Device;
struct ID3D12Resource;

namespace Influx
{
	class IndexBuffer final
	{
	public:
		using Index = uint32_t;
		static sPtr<IndexBuffer> Create();
		
		// Send internal data to GPU
		void Upload(Ptr<ID3D12GraphicsCommandList> cmdList, comPtr<ID3D12Device> device, Index const* pData, uint32_t n);

		D3D12_INDEX_BUFFER_VIEW GetView() const { return mIndexBufferView; }
		comPtr<ID3D12Resource> GetDxIndexBuffer() const;

		~IndexBuffer();

	private:
		IndexBuffer() = default;

		comPtr<ID3D12Resource> mpDxIndexBuffer;
		comPtr<ID3D12Resource> mpDxInterBuffer;
		D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
	};
}


