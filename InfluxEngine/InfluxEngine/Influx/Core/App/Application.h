#pragma once
#include <Windows.h>
#include "Influx/Core/Alias.h"
#include "Influx/Utils/Timer.h"

namespace Influx
{
	struct Window;
	class Renderer;

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
		AppState mState;
		Timer mTimer;

		sPtr<Window> mpWindow;
		sPtr<Renderer> mpRenderer;

		void Initialize(HINSTANCE i);
		void Update();

	public:
		static sPtr<Application> Create(); // define in client project

		void Run(HINSTANCE i);

	protected:
		sPtr<Renderer> GetRenderer() const;
	};
}


