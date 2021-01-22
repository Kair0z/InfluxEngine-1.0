#include "pch.h"
#include "DemoWidget.h"

namespace Influx::Widgets
{
	void DemoWidget::operator()()
	{
		ImGui::ShowDemoWindow();
	}
}

