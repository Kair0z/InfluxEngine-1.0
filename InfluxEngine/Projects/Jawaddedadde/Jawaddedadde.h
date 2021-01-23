#pragma once
#include "Influx.h"

using namespace Influx;

// ***********************************************
//  * Jawaddedadde *
// *********************************************** 

class Jawaddedadde final : 
	public Influx::Application
{
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

	// * Called at rendering Gui (Before the frame)
	virtual void OnGuiRender() const override final;
};

// [!] Override the main application to be created... [!]
sPtr<Application> Application::Create()
{
	return sPtr<Jawaddedadde>(new Jawaddedadde());
}



