
#include "Influx/Core/App/Application.h"

int main(int argc, char** argv)
{
	using namespace Influx;

	// Declared in App class... Defined in inheriting application project!
	sPtr<Application> app = Application::Create();
	app->Run();
}