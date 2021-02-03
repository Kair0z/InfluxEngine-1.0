#include "pch.h"
#include "Shader.h"
#include "Influx/Resources/ResourceManagement/ResourceManager.h"
#include "Influx/Graphics/DxLayer/DxLayer.h"
#include "Influx/Core/Utils/Utils.h"

namespace Influx
{
#pragma region LoadingShaders
	template<>
	static Shader* Loading::LoadResource(const std::string&)
	{
		return new Shader();
	}

	template<>
	static void Loading::UnloadResource(Shader* pShader)
	{
		delete pShader;
		pShader = nullptr;
	}
#pragma endregion

	Shader* Shader::Load(const std::string& fName, const Desc& desc)
	{
		Shader* pShader = ResourceManager::Load<Shader>(fName);
		DxLayer::CompileShader(Influx::s2w(fName).c_str(), desc.entrypoint.c_str(), GetShaderProfileString(desc.shaderProfile, desc.shaderType).c_str(), &pShader->mpCompiledShaderBlob);

		return pShader;
	}

	ID3DBlob* Shader::GetDxCompileBlob() const
	{
		return mpCompiledShaderBlob;
	}

	// [REV] There's gotta be a cleaner way to go about converting enum types to string...
	std::string Shader::GetShaderProfileString(Profile profile, Type type)
	{
		std::string profileString;
		std::string typeString;

		switch (type)
		{
		case Type::PS: typeString = "ps"; break;
		case Type::VS: typeString = "vs"; break;
		}

		switch (profile)
		{
		case Profile::PF_5_1: profileString = "_5_1"; break;
		}

		return typeString + profileString;
	}
}

