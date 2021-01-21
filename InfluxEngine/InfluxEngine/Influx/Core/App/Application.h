#pragma once
#include <Windows.h>
#include "Influx/Core/Alias.h"

namespace Influx
{
	struct Window;
	class Application
	{
	public:
		static sPtr<Application> Create();

		void Run(HINSTANCE i);

	private:
		sPtr<Window> mpWindow;

		void Initialize(HINSTANCE i);
		void Update();
	};
}


