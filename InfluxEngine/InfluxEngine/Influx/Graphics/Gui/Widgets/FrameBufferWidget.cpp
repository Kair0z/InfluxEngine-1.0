#include "pch.h"
#include "FrameBufferWidget.h"

#include "Influx/Graphics/DxLayer/Fbo/FrameBuffer.h"

namespace Influx
{
	namespace Widgets
	{
		FrameBufferWidget::FrameBufferWidget(sPtr<FrameBuffer> buffer)
		{
			auto gpuHandle = buffer->GetColorGpuHandle(0, FrameBuffer::ViewType::SRV);
			ImGui::Image((void*)gpuHandle.ptr, { 200, 200 });
		}
	}
}

