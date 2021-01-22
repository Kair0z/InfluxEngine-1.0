#pragma once
#include "BaseWidget.h"

namespace Influx::Widgets
{
	struct DemoWidget final : public BaseWidget
	{
		virtual void operator()() override;
	};
}

