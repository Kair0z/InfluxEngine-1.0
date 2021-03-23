#pragma once
#include "Influx/Graphics/DxLayer/Resource/DxResource.h"

namespace Influx
{
	class Texture final : public DxResource
	{
	public:
		struct Desc
		{
			Vector2u dimensions{};
			// [TODO]
			// miplevels
			// samplecount
			// depth | arraySize

			DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
			DxResource::BindFlags bindFlags = DxResource::BindFlags::ShaderResource;
		};
		static sPtr<Texture> Create(const Desc& desc);
		static sPtr<Texture> CreateFromFile(const std::string filename, DxResource::BindFlags bindFlags = DxResource::BindFlags::ShaderResource);
		
		// void GetSRV();
		// void GetUAV();
		// void GetDSV();

	private:
		Texture() = default;
	};
}


