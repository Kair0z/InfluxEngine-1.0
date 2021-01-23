#include "pch.h"
#include "IndexBuffer.h"

namespace Influx
{
	sPtr<IndexBuffer> IndexBuffer::Create()
	{
		sPtr<IndexBuffer> idxBuffer(new IndexBuffer());

		return idxBuffer;
	}

	IndexBuffer::~IndexBuffer()
	{
	}
}

