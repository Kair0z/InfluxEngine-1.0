#pragma once

namespace Influx
{
	class IndexBuffer final
	{
	public:
		static sPtr<IndexBuffer> Create();
		~IndexBuffer();

	private:
		IndexBuffer() = default;

		comPtr<ID3D12Resource> mpDxIndexBuffer;
		D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
	};
}


