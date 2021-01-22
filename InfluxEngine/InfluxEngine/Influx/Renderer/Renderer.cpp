#include "pch.h"
#include "Renderer.h"

namespace Influx
{
	sPtr<Renderer> Influx::Renderer::Create(const Desc& desc)
	{
		return sPtr<Renderer>(new Renderer());
	}

	void Renderer::Initialize()
	{
		if (mIsInitialized) return;

		// Enable the Debug layer if in debug
#ifdef _DEBUG
		DxLayer::EnableDebugLayer();
#endif

		// Create DxLayer Instance
		DxLayer::Desc dxDesc;
		dxDesc.mUseWarp = true;
		mpDx = sPtr<DxLayer>(DxLayer::LoadDX12(dxDesc));

		mIsInitialized = true;
	}

	void Renderer::Shutdown()
	{
		// Release all resources:
		//...

		// Check for live objects: 
		// (TODO: remove device from this report-list as it's obviously not yet released)
		DxLayer::ReportLiveObjects(mpDx->GetDevice());

		// Release Device happens in smartpointer destructor...
	}
}

