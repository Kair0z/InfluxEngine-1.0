#pragma once
#include <stdint.h>

namespace Influx
{
	// [WIP] Wrapper class for ID3D12Resource API
	class DxResource
	{
	public:
		enum class Type
		{
			Buffer,
			Texture1D,
			Texture2D
			// Texture3D,
			// TextureCube,
		};

		// Our version of ID3D12_RESOURCE_STATE...
		enum class ResourceState
		{
			Undefined
		};
		enum class BindFlags : uint32_t
		{
			None = 0x0,
			Constant = 0x1,
			ShaderResource = 0x2,
			UnorderedAccess = 0x4,
			RenderTarget = 0x8,
			DepthStencil = 0x10
		};

	private:

	};
}


