#pragma once
#include "Influx/Core/Alias.h"

namespace Influx
{
	class Application
	{
	public:
		static sPtr<Application> Create();

		void Run();
	};
}


