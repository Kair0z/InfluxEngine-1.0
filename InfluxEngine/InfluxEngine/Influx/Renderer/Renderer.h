#pragma once
#include "Influx/Core/Alias.h"
#include "DxLayer/DxLayer.h"

namespace Influx
{
	class Renderer final
	{
	public:
		struct Desc
		{

		};
		static sPtr<Renderer> Create(const Desc& desc);

		void Initialize();

		~Renderer() { Shutdown(); }

	private:
		Renderer() = default;
		bool mIsInitialized = false;

		sPtr<DxLayer> mpDx;

		void Shutdown();
	};
}


