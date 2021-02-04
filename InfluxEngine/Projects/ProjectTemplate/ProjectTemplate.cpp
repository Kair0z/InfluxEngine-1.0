#include "ProjectTemplate.h"

#include "Influx/Graphics/Gui/Widgets/DemoWidget.h"
#include "Influx/Graphics/Gui/Widgets/FrameBufferWidget.h"

using namespace Influx;
void ProjectTemplate::OnStart()
{
	FrameBuffer::Desc framebufferDesc;
	framebufferDesc.dimensions = GetAppInfo().windowDimensions;
	framebufferDesc.nColorTargets = 4;
	framebufferDesc.useDepthTarget = true;

	mpScene = Scene::Create("../InfluxEngine/Resources/Meshes/CubeScene.fbx");

	mpBuffer = FrameBuffer::Create(framebufferDesc);
	mpRasterizer = RasterPass::Create();
	mpRasterizer->SetScene(mpScene);
}

void ProjectTemplate::OnFrame()
{
	mpRasterizer->Execute(mpBuffer);
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
