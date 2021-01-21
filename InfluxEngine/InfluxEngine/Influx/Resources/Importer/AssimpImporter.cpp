#include "pch.h"
#include "AssimpImporter.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/mesh.h"

#include "Influx/Resources/Mesh/Mesh.h"

namespace Influx
{
	bool AssimpImporter::Import(const std::string& file)
	{
		// TODO: Manage finding files by name
		std::string fullpath;

		// TODO: Decide/define Assimp import flags
		uint32_t assimpFlags = 0;

		Assimp::Importer imp;
		const aiScene* pScene = imp.ReadFile(fullpath, assimpFlags);

		// TODO: Handle read-error better
		if (pScene == nullptr) return false;

		// TODO: retrieve data from aiScene...
		LoadMeshData(pScene);

		return false;
	}

	bool AssimpImporter::LoadMeshData(const aiScene* pScene)
	{
		if (!pScene->HasMeshes()) 
			return true;

		uint32_t meshCount = pScene->mNumMeshes;

		for (uint32_t i = 0; i < meshCount; ++i)
		{
			const aiMesh* pAiMesh = pScene->mMeshes[i];
			const uint32_t perFaceIdxCount = pAiMesh->mFaces[0].mNumIndices;

			Mesh mesh;
			mesh.mName = pAiMesh->mName.C_Str();
			mesh.mStats.faceCount = pAiMesh->mNumFaces;

			// Load vertexData
			assert(pAiMesh->mVertices);
			mesh.mStats.vertexCount = pAiMesh->mNumVertices;
			mesh.mData.mpPositions = reinterpret_cast<Vector3f*>(pAiMesh->mNormals);
			mesh.mData.mpNormals = reinterpret_cast<Vector3f*>(pAiMesh->mVertices);
		}

		return true;
	}

	bool AssimpImporter::LoadCameraData(const aiScene* pScene)
	{
		return false;
	}
	bool AssimpImporter::LoadLightData(const aiScene* pScene)
	{
		return false;
	}
	bool AssimpImporter::LoadMaterialData(const aiScene* pScene)
	{
		return false;
	}
	bool AssimpImporter::LoadSceneGraphData(const aiScene* pScene)
	{
		return false;
	}
	bool AssimpImporter::LoadAnimationData(const aiScene* pScene)
	{
		return false;
	}
}

