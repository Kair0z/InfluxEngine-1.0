#include "ProjectTemplate.h"

#include "Influx/Graphics/Gui/Widgets/DemoWidget.h"
#include "Influx/Graphics/Gui/Widgets/FrameBufferWidget.h"

using namespace Influx;
void ProjectTemplate::OnStart()
{
	FrameBuffer::Desc framebufferDesc;
	framebufferDesc.dimensions = GetInfo().windowDimensions;
	framebufferDesc.nColorTargets = 4;
	framebufferDesc.useDepthTarget = true;

	mpBuffer = FrameBuffer::Create(framebufferDesc);
	//mpRasterizer = RasterPass::Create();
}

void ProjectTemplate::OnFrame()
{
	//mpRasterizer->Execute(mpBuffer);
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
	Widgets::FrameBufferWidget bff(mpBuffer);
	//Widgets::DemoWidget();
}
