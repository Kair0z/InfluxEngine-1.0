#include "pch.h"
#include "AssimpImporter.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/mesh.h"

#include "Influx/Scene/SceneData.h"

namespace Influx
{
	bool AssimpImporter::Import(const std::string& file, SceneData& builder)
	{
		// [TODO]: Manage finding files by name
		std::string fullpath = file;

		// [TODO]: Decide/define Assimp import flags
		uint32_t assimpFlags = 0;

		Assimp::Importer imp;
		const aiScene* pScene = imp.ReadFile(fullpath, assimpFlags);
		if (pScene == nullptr) return false;
		// [TODO]: Handle read-error better...

		// [TODO]: retrieve data from aiScene...
		LoadMeshData(pScene, builder);

		return true;
	}

	bool AssimpImporter::LoadMeshData(const aiScene* pScene, SceneData& data)
	{
		if (!pScene->HasMeshes()) 
			return true;

		uint32_t meshCount = pScene->mNumMeshes;

		for (uint32_t i = 0; i < meshCount; ++i)
		{
			const aiMesh* pAiMesh = pScene->mMeshes[i];
			const uint32_t perFaceIdxCount = pAiMesh->mFaces[0].mNumIndices;

			Influx::Mesh mesh{};
			mesh.mName = pAiMesh->mName.C_Str();

			for (uint32_t v = 0; v < pAiMesh->mNumVertices; ++v)
			{
				Influx::Vertex vtx{};

				// Vertex Positions:
				auto aiPos = pAiMesh->mVertices[v];
				vtx.mPosition = { aiPos.x, aiPos.y, aiPos.z };

				// Vertex Colors:
				if (pAiMesh->HasVertexColors(0))
				{
					auto aiColor = pAiMesh->mColors[v][0];
					vtx.mColor = { aiColor.r, aiColor.g, aiColor.b };
				}
				
				mesh.mVertices.push_back(vtx);
			}
			data.mMeshes.push_back(mesh);
		}

		return true;
	}
}

