#pragma once
#include "Influx/Resources/ResourceManagement/Loading.h"

namespace Influx
{	
	struct Shader final
	{
	public:
		enum class Profile // [REV] Adding profiles
		{
			PF_5_1 
		};
		enum class Type // [REV] Adding types
		{
			VS,
			PS
		};

		struct Desc
		{
			Desc(const std::string entry, Type t, Profile p) : entrypoint{entry}, shaderType{t}, shaderProfile{p}{}
			std::string entrypoint;
			Type shaderType;
			Profile shaderProfile;
		};
		static Shader* Load(const std::string& fName, const Desc& desc);

		ID3DBlob* GetDxCompileBlob() const;

	private:
		ID3DBlob* mpCompiledShaderBlob;

		IFX_DeclLoadingFriend(Shader);
		Shader() = default;

		static std::string GetShaderProfileString(Profile profile, Type type);
	};


}


