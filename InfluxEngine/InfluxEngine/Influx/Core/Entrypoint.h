#pragma once
#include "pch.h"
#include "Influx/Core/App/Application.h"
#include "Influx/Graphics/DxLayer/DxLayer.h"

int CALLBACK wWinMain(HINSTANCE i, HINSTANCE, PWSTR, int)
{
	using namespace Influx;

	// This reports 'fake d3d12 leaks'
	DxLayer::EnableDebugLayer();

	{
		// Declared in App class... Defined in inheriting application project!
		sPtr<Application> app = Application::Create();

		// Load the app inside the Service locator
		LocateApp::Load(app);

		app->Run(i);
	}

	DxLayer::ReportLiveObjects();
}