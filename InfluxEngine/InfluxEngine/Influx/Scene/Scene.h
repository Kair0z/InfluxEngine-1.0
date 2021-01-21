#pragma once
#include "Influx/Alias.h"

namespace Influx
{
	class Scene final
	{
	public:
		static sPtr<Scene> Create();

	public:
		enum class UpdateFlags
		{
			None = 0x0,
			MeshesMoved = 0x1,
			CameraMoved = 0x2,
			LightsMoved = 0x10,

			All = -1
		};
		struct SceneStats;
		
	private:
		Scene() = default;

		// Data:
		// Mesh Data...

		// Material Data...

		// Lights Data...

		// Camera Data...
	};

	struct Scene::SceneStats
	{
		ui64 uTriangleCount = 0;
		ui64 uVertexCount = 0;
		//...
	};
}


