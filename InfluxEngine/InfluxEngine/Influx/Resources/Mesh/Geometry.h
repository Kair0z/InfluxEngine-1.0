#pragma once
#include <Influx/Math/Math.h>
#include <vector>

#include "Influx/Graphics/DxLayer/VertexBuffer.h"
#include "Influx/Graphics/DxLayer/IndexBuffer.h"

namespace Influx
{
	struct Vertex
	{
		Vector3f mPosition;
		Vector3f mColor;
	};

	struct Mesh final
	{
		std::string mName;
		std::vector<Vertex> mVertices;
	};
}


