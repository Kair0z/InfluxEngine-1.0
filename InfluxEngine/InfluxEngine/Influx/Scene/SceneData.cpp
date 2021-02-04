#include "pch.h"
#include "SceneData.h"
#include "Influx/Resources/Importer/AssimpImporter.h"

#include "Influx/Resources/ResourceManagement/Loading.h"

namespace Influx::Loading
{
	template<>
	SceneData* LoadResource(const std::string& fName)
	{
		SceneData* pResult = new SceneData();
		AssimpImporter::Import(fName, *pResult);

		return pResult;
	}

	template <>
	void UnloadResource(SceneData* pObj)
	{
		delete pObj;
		pObj = nullptr;
	}
}