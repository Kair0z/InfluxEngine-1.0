#pragma once
#include "Influx.h"

using namespace Influx;

class Sandbox final : 
	public Influx::Application
{
public:
	Sandbox() = default;

private:
	// * Called at Application start
	virtual void OnStart() override final;

	// * Called before rendering a frame
	virtual void OnFrame() override final;

	// * Called at Application exit
	virtual void OnShutdown() override final;

	// * Called at Input Events
	virtual void OnKeyEvent() override final;
	virtual void OnMouseEvent() override final;
};

// [!] Override the main application to create... [!]
sPtr<Application> Application::Create()
{
	return sPtr<Sandbox>(new Sandbox());
}



