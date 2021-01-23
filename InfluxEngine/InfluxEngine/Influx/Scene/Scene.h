#pragma once
#include "Influx/Core/Alias.h"

namespace Influx
{
	class Scene final
	{
	public:
		static sPtr<Scene> Create();
		static sPtr<Scene> Create(const std::string& filename);
		IFX_DelCpyMove(Scene);

	public:
		enum class UpdateFlags
		{
			None = 0x0,
			MeshesMoved = 0x1,
			CameraMoved = 0x2,
			LightsMoved = 0x10,

			All = -1
		};
		struct SceneStats
		{
			ui64 uTriangleCount = 0;
			ui64 uVertexCount = 0;
			//...
		};
		
	private:
		Scene() = default;

		// Data:
		// Mesh Data...

		// Material Data...

		// Lights Data...

		// Camera Data...
	};

	
}


