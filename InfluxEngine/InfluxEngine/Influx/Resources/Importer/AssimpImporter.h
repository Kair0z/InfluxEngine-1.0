#pragma once
#include <string>

struct aiScene;

namespace Influx
{
	struct SceneData;
	class AssimpImporter final
	{
	public:
		static bool Import(const std::string& file, SceneData& sceneData);

	private:
		static bool LoadMeshData(const aiScene* pScene, SceneData& sceneData);

		// TODO:
		/*static bool LoadMaterialData(const aiScene* pScene, SceneBuilder& builder);
		static bool LoadCameraData(const aiScene* pScene, SceneBuilder& builder);
		static bool LoadLightData(const aiScene* pScene, SceneBuilder& builder);
		static bool LoadSceneGraphData(const aiScene* pScene, SceneBuilder& builder);
		static bool LoadAnimationData(const aiScene* pScene, SceneBuilder& builder);*/
	};
}


