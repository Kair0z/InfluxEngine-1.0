#pragma once
#include "pch.h"
#include "Influx/Core/App/Application.h"
#include "Influx/Graphics/DxLayer/DxLayer.h"

int CALLBACK wWinMain(HINSTANCE i, HINSTANCE, PWSTR, int)
{
	using namespace Influx;

	//NOTE: DebugLayer has to be activated before initializing the device, else it will invalide the active device
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