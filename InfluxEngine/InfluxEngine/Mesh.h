#pragma once
#include <string>
#include <Influx/Alias.h>

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

		};
		Stats mStats;
		

	private:
	};


	
}


