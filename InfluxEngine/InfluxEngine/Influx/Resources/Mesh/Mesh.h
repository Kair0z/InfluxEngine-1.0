#pragma once
#include <string>
#include <Influx/Core/Alias.h>
#include <Influx/Resources/Mesh/Vertex.h>

namespace Influx
{
	struct Mesh final
	{
	public:
		template <typename T>
		struct Attribute;

		std::string mName;

		struct Stats
		{
			ui32 faceCount;
			ui32 vertexCount;
			ui32 indexCount;
		};
		struct Data
		{
			Vector3f* mpPositions;
			Vector3f* mpNormals;
			Vector2f* mpTexCoords;
			//Bones?
		};

		Stats mStats;
		Data mData;
	};


	
}


