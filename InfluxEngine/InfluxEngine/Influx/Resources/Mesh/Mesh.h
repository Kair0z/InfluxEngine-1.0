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
			Vector3f mPosition;
			Vector3f mNormal;
			Vector2f mTexCoord;
			//Bones?
		};

		Stats mStats;
		Data mData;
	};


	
}


