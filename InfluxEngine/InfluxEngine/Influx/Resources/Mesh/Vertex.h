#pragma once
#include <Influx/Math/Math.h>

namespace Influx
{
	struct Vertex
	{
		Vector3f mPosition;
		Vector3f mNormal;
		Vector3f mTangent;
		Vector2f mTexCoord;
		// bone data...
	};

	struct ShVertex
	{
		Vector3f mPosition;
		Vector3f mColor;
	};
}


