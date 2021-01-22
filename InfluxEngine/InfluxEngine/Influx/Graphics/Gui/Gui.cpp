#include "pch.h"
#include "Gui.h"

#include "../DxLayer/DxLayer.h"

namespace Influx
{
	// Global OnGuiRender Callback:
	Action Gui::sOnGuiRender{};

	ID3D12DescriptorHeap* Gui::spFontSrvDescHeap = nullptr;

	void Gui::Initialize(const Dx12Impl_Desc& dxDesc, ID3D12Device* device)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui_ImplWin32_Init(dxDesc.windowHandle);

		spFontSrvDescHeap = DxLayer::CreateDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		ImGui_ImplDX12_Init(device,
			dxDesc.nBuffers, 
			dxDesc.bufferFormat, 
			spFontSrvDescHeap,
			spFontSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
			spFontSrvDescHeap->GetGPUDescriptorHandleForHeapStart());
	}

	void Gui::Render()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Call On Gui Render signal:
		sOnGuiRender();

		ImGui::Render();
	}

	void Gui::Cmd_SubmitDrawData(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->SetDescriptorHeaps(1, &spFontSrvDescHeap);
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
	}

	void Gui::Cleanup()
	{
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		spFontSrvDescHeap->Release();
	}

	void Gui::ListenOnRender(const std::function<void()>& f)
	{
		sOnGuiRender.AddListener(f);
	}
}

