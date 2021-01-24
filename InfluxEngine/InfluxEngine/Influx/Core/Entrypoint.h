#pragma once
#include "pch.h"
#include "Influx/Core/App/Application.h"
#include "Influx/Graphics/DxLayer/DxLayer.h"

int CALLBACK wWinMain(HINSTANCE i, HINSTANCE, PWSTR, int)
{
	using namespace Influx;

	// Declared in App class... Defined in inheriting application project!
	sPtr<Application> app = Application::Create();
	app->Run(i);

	DxLayer::ReportLiveObjects();
}