#include "AssimpImporter.h"

#include "assimp/Importer.hpp"

namespace Influx
{
	bool AssimpImporter::Import(const std::string& file)
	{
		// TODO: Manage finding files by name
		std::string fullpath;

		// TODO: Decide/define Assimp import flags
		uint32_t assimpFlags = 0;

		Assimp::Importer imp;
		const aiScene* pScene = imp.ReadFile(fullpath, assimpFlags);

		// TODO: Handle read-error better
		if (pScene == nullptr) return false;

		// TODO: retrieve data from aiScene...

		return false;
	}
}

