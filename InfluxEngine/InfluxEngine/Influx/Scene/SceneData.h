#pragma once
#include "Influx/Resources/Mesh/Geometry.h"

/// <summary>
/// The raw data from an scene file that must only be loaded once via the resourcemanager: 
/// </summary>
namespace Influx
{
	struct SceneData final
	{
	public:
		std::vector<Mesh> mMeshes;
	};
}


