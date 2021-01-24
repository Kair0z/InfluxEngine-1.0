#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <Influx/Core/Alias.h>

namespace Influx
{
	struct DxLayer final
	{
	public:
		struct Desc { bool mUseWarp; };
		static Ptr<DxLayer> LoadDX12(const Desc& desc);
		IFX_DelCpyMove(DxLayer);

		static HRESULT CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** shaderBlob);

		static D3D12_VERSIONED_ROOT_SIGNATURE_DESC CreateRootSignatureDesc(const CD3DX12_ROOT_PARAMETER1* rootParams, D3D12_ROOT_SIGNATURE_FLAGS flags);
		static Ptr<ID3D12RootSignature> CreateRootSignature(Ptr<ID3D12Device2> device, const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pDesc);

		static Ptr<ID3D12CommandQueue> CreateCommandQueue(Ptr<ID3D12Device2> pDevice, D3D12_COMMAND_LIST_TYPE type);
		static Ptr<IDXGISwapChain4> CreateSwapChain(HWND wndHandle, Ptr<ID3D12CommandQueue> commandQueue, uint32_t w, uint32_t h, uint32_t bffCount);
		static Ptr<ID3D12DescriptorHeap> CreateDescriptorHeap(Ptr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
		static Ptr<ID3D12CommandAllocator> CreateCommandAllocator(Ptr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);
		static Ptr<ID3D12GraphicsCommandList> CreateCommandList(Ptr<ID3D12Device2> device, Ptr<ID3D12CommandAllocator> alloc, D3D12_COMMAND_LIST_TYPE type);
		static Ptr<ID3D12PipelineState> CreatePipelineState(Ptr<ID3D12Device2> device, const D3D12_PIPELINE_STATE_STREAM_DESC& desc);
		static void	LoadBufferResource(Ptr<ID3D12Device> device, Ptr<ID3D12GraphicsCommandList> cmdList, ID3D12Resource** destResource, ID3D12Resource** intResource, size_t nElements, size_t elementSize, const void* pData, D3D12_RESOURCE_FLAGS flags);

		using FenceEvent = HANDLE;
		static Ptr<ID3D12Fence> CreateFence(Ptr<ID3D12Device2> device);
		static FenceEvent CreateFenceEventHandle();
		static uint64_t SignalFence(Ptr<ID3D12CommandQueue> cmdQueue, Ptr<ID3D12Fence> fence, uint64_t& fenceValue);
		// If unlockvalue is never reached, cpu thread will block for min. 584 million years
		static void WaitForFenceValue(Ptr<ID3D12Fence> fence, uint64_t unlockValue, FenceEvent e, std::chrono::milliseconds duration = std::chrono::milliseconds::max());
		static bool IsFenceComplete(Ptr<ID3D12Fence> fence, uint64_t unlockValue);
		// This will block the calling thread until the fence val has been reached. Afterwards, it's safe to release any resources referenced in GPU
		static void Flush(Ptr<ID3D12CommandQueue> cmdQueue, Ptr<ID3D12Fence> fence, uint64_t& fenceValue, FenceEvent e);

		static void Cmd_TransitionResource(Ptr<ID3D12GraphicsCommandList2> cmdList, Ptr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

		static bool CheckTearingSupport();
		static void EnableDebugLayer();
		static void ReportLiveObjects();

		comPtr<IDXGIAdapter4> GetAdapter() const { return mpAdapter; }
		comPtr<ID3D12Device2> GetDevice() const { return mpDevice; }

	private:
		comPtr<IDXGIAdapter4> mpAdapter;
		comPtr<ID3D12Device2> mpDevice;

		std::vector<ID3D12DescriptorHeap> mpDescHeaps;

		static Ptr<IDXGIAdapter4> FindAdapter(bool useWarp);
		static Ptr<ID3D12Device2> CreateDevice(Ptr<IDXGIAdapter4> adapter);

		void InitInfoQueue();

	private:
		DxLayer() = default;

	public:
		~DxLayer()
		{
			mpDevice->Release();
			mpAdapter->Release();
		}
	};
}

