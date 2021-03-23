#include "pch.h"
#include "Scene.h"
#include "SceneData.h"
#include "Influx/Resources/ResourceManagement/ResourceManager.h"

namespace Influx
{
	sPtr<Scene> Scene::Create()
	{
		// Creating a scene with empty scene data:
		sPtr<Scene> scene(new Scene());
		scene->mpSceneData = sPtr<SceneData>(new SceneData());

		return scene;
	}
	sPtr<Scene> Scene::Create(const std::string& file)
	{
		// Creating a scene with scene data provided from resource manager [loaded once]
		sPtr<Scene> scene(new Scene());
		scene->mpSceneData = sPtr<SceneData>(ResourceManager::Load<SceneData>(file));

		return scene;
	}

	const std::vector<Mesh>& Scene::GetMeshes() const
	{
		return mpSceneData->mMeshes;
	}
}

