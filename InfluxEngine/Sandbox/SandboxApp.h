#pragma once
#include "Influx.h"

Influx::sPtr<Influx::Application> Influx::Application::Create()
{
	return std::make_shared<Influx::Application>();
}

class SandboxApp final : 
	public Influx::Application
{
public:

private:
};




