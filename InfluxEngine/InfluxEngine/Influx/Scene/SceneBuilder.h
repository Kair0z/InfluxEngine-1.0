#pragma once
#include "Influx/Alias.h"

/// <summary>
/// This Helper class defines how the scene should get built
/// Making use of flags and additional settings...
/// </summary>

namespace Influx
{
	class SceneBuilder final
	{
	public:
		enum class Flags;

	private:
	};

	enum class SceneBuilder::Flags
	{
		None = 0x0,
		Default = None
	};
}


