#pragma once
#include <vector>
#include "ILoader.h"

namespace Influx
{
	class ResourceManager final
	{
	public:
		ResourceManager() = default;
		~ResourceManager();

		static void AddLoader(Intern::ILoader* pLoader);
		
		template <class Resource>
		static Resource* Load(const std::string& fName)
		{
			for (Intern::ILoader* pLoader : statLoaders)
			{
				if (typeid(Resource) != pLoader->GetType()) continue;

				return static_cast<Loader<Resource>*>(pLoader)->GetResource(fName);
			}

			// If such loader doesn't exist yet, create a new one...
			Loader<Resource>* pNewLoader = new Loader<Resource>();
			if (!pNewLoader) return nullptr;
			AddLoader(pNewLoader);

			return pNewLoader->GetResource(fName);
		}

	private:
		static std::vector<Intern::ILoader*> statLoaders;
	};

	inline ResourceManager::~ResourceManager()
	{
		for (Intern::ILoader* pLoader : statLoaders)
			delete pLoader;
	}
	inline void ResourceManager::AddLoader(Intern::ILoader* pLoader)
	{
		if (!pLoader) return;
		statLoaders.push_back(pLoader);
	}

	std::vector<Intern::ILoader*> ResourceManager::statLoaders{};
}



