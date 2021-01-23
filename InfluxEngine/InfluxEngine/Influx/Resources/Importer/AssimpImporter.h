#pragma once
#include <string>

struct aiScene;

namespace Influx
{
	struct SceneBuilder;
	class AssimpImporter final
	{
	public:
		static bool Import(const std::string& file, SceneBuilder& builder);

	private:
		static bool LoadMeshData(const aiScene* pScene, SceneBuilder& builder);

		// TODO:
		/*static bool LoadMaterialData(const aiScene* pScene, SceneBuilder& builder);
		static bool LoadCameraData(const aiScene* pScene, SceneBuilder& builder);
		static bool LoadLightData(const aiScene* pScene, SceneBuilder& builder);
		static bool LoadSceneGraphData(const aiScene* pScene, SceneBuilder& builder);
		static bool LoadAnimationData(const aiScene* pScene, SceneBuilder& builder);*/
	};
}


