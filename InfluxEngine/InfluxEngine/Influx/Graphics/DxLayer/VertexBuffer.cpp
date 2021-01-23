#include "pch.h"
#include "VertexBuffer.h"

namespace Influx
{
	sPtr<VertexBuffer> Influx::VertexBuffer::Create()
	{
		sPtr<VertexBuffer> vertexBuffer(new VertexBuffer());

		return vertexBuffer;
	}

	Influx::VertexBuffer::~VertexBuffer()
	{
	}
}

