#pragma once
#include <Windows.h>
#include "Influx/Core/Utils/Timer.h"

namespace Influx
{
	struct Window;
	class Renderer;
	struct DxLayer;
	class QueueManager;

	class Application 
	{
		enum class AppState
		{
			None, Uninitialized, Running
		};

	private:
		// Inheritable events:
		virtual void OnStart() {};
		virtual void OnFrame() {};
		virtual void OnShutdown() {};
		virtual void OnKeyEvent() {};
		virtual void OnMouseEvent() {};

		virtual void OnGuiRender() const {};

	private:
		sPtr<Window> mpWindow;
		sPtr<DxLayer> mpDx;
		sPtr<QueueManager> mpQueueManager;

		Timer mTimer;
		AppState mState;

		void Initialize(HINSTANCE i);

		void Update();

	public:
		static sPtr<Application> Create(); // This gets defined in client project
		void Run(HINSTANCE i);
	};
}

#include "Influx/Core/Utils/ServiceLocator.h"
class LocateApp : public Influx::ServiceLocator<Influx::Application> {};

