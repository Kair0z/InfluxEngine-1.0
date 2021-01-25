#include "ProjectTemplate.h"
#include "Influx/Graphics/Gui/Widgets/DemoWidget.h"

using namespace Influx;

void ProjectTemplate::OnStart()
{
}

void ProjectTemplate::OnFrame()
{
}

void ProjectTemplate::OnShutdown()
{
}

void ProjectTemplate::OnKeyEvent()
{
}

void ProjectTemplate::OnMouseEvent()
{
}

void ProjectTemplate::OnGuiRender() const
{
	Widgets::DemoWidget();
}
