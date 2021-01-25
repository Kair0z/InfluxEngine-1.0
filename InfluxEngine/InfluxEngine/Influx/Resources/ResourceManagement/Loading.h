#pragma once
#include <string>

#pragma warning(push)
#pragma warning (disable: 4505)

namespace Influx
{
	namespace Loading
	{
		template <class Resource>
		static Resource* LoadResource(const std::string& fName);

		template <class Resource>
		static void UnloadResource(Resource* pObj);
	}
}

//#include "MeshLoading.h"
//#include "TextureLoading.h"

#pragma warning(pop)