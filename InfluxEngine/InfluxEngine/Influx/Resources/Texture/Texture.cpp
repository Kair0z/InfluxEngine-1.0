#include "pch.h"
#include "Texture.h"


namespace Influx
{
	sPtr<Texture> Influx::Texture::Create(const Desc&)
	{
		sPtr<Texture> texture(new Texture());

		return texture;
	}
}

