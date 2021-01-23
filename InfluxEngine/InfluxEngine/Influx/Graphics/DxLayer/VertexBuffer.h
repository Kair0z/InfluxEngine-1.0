#pragma once

namespace Influx
{
	class VertexBuffer final
	{
	public:
		static sPtr<VertexBuffer> Create();
		~VertexBuffer();

	private:
		VertexBuffer() = default;

		comPtr<ID3D12Resource> mpDxVertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	};
}


