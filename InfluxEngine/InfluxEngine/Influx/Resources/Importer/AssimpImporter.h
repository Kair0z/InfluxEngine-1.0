#pragma once
#include <string>

#pragma comment(lib, "assimp.lib")

struct aiScene;

namespace Influx
{
	class AssimpImporter final
	{
	public:
		static bool Import(const std::string& file);

	private:
		static bool LoadMeshData(const aiScene* pScene);

		// TODO:
		static bool LoadMaterialData(const aiScene* pScene);
		static bool LoadCameraData(const aiScene* pScene);
		static bool LoadLightData(const aiScene* pScene);
		static bool LoadSceneGraphData(const aiScene* pScene);
		static bool LoadAnimationData(const aiScene* pScene);
	};
}


