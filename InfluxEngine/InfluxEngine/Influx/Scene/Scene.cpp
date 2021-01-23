#include "pch.h"
#include "Scene.h"

namespace Influx
{
	sPtr<Scene> Scene::Create()
	{
		return sPtr<Scene>(new Scene());
	}

	sPtr<Scene> Scene::Create(const std::string& filename)
	{
		return sPtr<Scene>();
	}
}

