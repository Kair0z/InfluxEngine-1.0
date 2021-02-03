#pragma once
#include <string>

#pragma warning(push)
#pragma warning (disable: 4505)

// Declares allowance for creation & destruction in templated load function...
#define IFX_DeclLoadingFriend(clsName) \
	friend clsName* Loading::LoadResource(const std::string& fName); \
	friend void Loading::UnloadResource(clsName* pObj);

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

#pragma warning(pop)