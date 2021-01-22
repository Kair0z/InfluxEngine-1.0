#pragma once
#include <functional>

struct ID3D12DescriptorHeap;

namespace Influx
{
	struct Gui
	{
	public:
		struct Dx12Impl_Desc
		{
			HWND windowHandle;
			DXGI_FORMAT bufferFormat;
			uint8_t nBuffers;
		};
		static void Initialize(const Dx12Impl_Desc& dxDesc, ID3D12Device* device);

		static void Render();
		static void Cmd_SubmitDrawData(ID3D12GraphicsCommandList* cmdList);

		static void Cleanup();

		static void ListenOnRender(const std::function<void()>& f);

	private:
		static Ptr<ID3D12DescriptorHeap> spFontSrvDescHeap;
		static Action sOnGuiRender;
	};
}


