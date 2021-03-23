#pragma once
#include <unordered_map>
#include "Loading.h"

namespace Influx
{
	namespace Intern
	{
		class ILoader
		{
		public:
			virtual ~ILoader() = default;
			virtual const type_info& GetType() const = 0;
		};
	}

	template <class Resource>
	class Loader : public Intern::ILoader
	{
	public:
		~Loader();
		virtual Resource* GetResource(const std::string& fName);
		virtual const type_info& GetType() const override final;
		virtual Resource* LoadResource(const std::string& fName);

	protected:
		virtual void Unload(Resource* pObj);
		std::unordered_map<std::string, Resource*> mContentMap;
	};

#pragma region Impl
	template<class Resource>
	inline Loader<Resource>::~Loader()
	{
		for (auto& pair : mContentMap)
			Loading::UnloadResource<Resource>(pair.second);
	}

	template<class Resource>
	inline const type_info& Loader<Resource>::GetType() const
	{
		return typeid(Resource);
	}

	template<class Resource>
	inline Resource* Loader<Resource>::LoadResource(const std::string& fName)
	{
		return Loading::LoadResource<Resource>(fName);
	}

	template<class Resource>
	inline void Loader<Resource>::Unload(Resource* pObj)
	{
		Loading::UnloadResource<Resource>(pObj);
	}

	template<class Resource>
	inline Resource* Loader<Resource>::GetResource(const std::string& fName)
	{
		// If the content doesn't yet exists load & return it...
		if (!mContentMap[fName]) 
			mContentMap[fName] = LoadResource(fName);

		return mContentMap[fName];
	}
#pragma endregion
}


