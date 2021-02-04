#pragma once
#include <Influx/Math/Math.h>
#include <vector>

namespace Influx
{
	struct Vertex
	{
		Vector3f mPosition;
		Vector3f mColor;
	};

	struct Mesh
	{
		std::string mName;
		std::vector<Vertex> mVertices;
	};
}


