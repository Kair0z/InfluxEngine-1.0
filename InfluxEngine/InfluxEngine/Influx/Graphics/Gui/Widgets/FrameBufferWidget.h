#pragma once

namespace Influx
{
	class FrameBuffer;
	namespace Widgets
	{
		struct FrameBufferWidget final
		{
			FrameBufferWidget(sPtr<FrameBuffer> buffer);
		};
	}
}


