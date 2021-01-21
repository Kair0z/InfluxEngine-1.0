#pragma once
#include <string>

#pragma comment(lib, "assimp.lib")

namespace Influx
{
	class AssimpImporter final
	{
	public:
		static bool Import(const std::string& file);
	};
}


