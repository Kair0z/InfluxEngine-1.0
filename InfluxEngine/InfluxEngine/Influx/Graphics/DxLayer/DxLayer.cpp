#include "pch.h"
#include "DxLayer.h"

namespace Influx
{
	Ptr<DxLayer> DxLayer::LoadDX12(const Desc& desc)
	{
		Ptr<DxLayer> layer = new DxLayer();

		layer->mpAdapter = comPtr<IDXGIAdapter4>(FindAdapter(desc.mUseWarp));
		layer->mpDevice = comPtr<ID3D12Device2>(CreateDevice(layer->mpAdapter.Get()));

		layer->InitInfoQueue();

		return layer;
	}

	HRESULT DxLayer::CompileShader(LPCWSTR srcFile, LPCSTR entryPoint, LPCSTR profile, ID3DBlob** sBlob)
	{
		if (!srcFile || !entryPoint || !profile || !sBlob) return E_INVALIDARG;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		HRESULT hr = D3DCompileFromFile(srcFile, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, profile,
			flags, 0, &shaderBlob, &errorBlob);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}

			if (shaderBlob)
				shaderBlob->Release();

			return hr;
		}

		*sBlob = shaderBlob;
		return hr;
	}

	D3D12_VERSIONED_ROOT_SIGNATURE_DESC DxLayer::CreateRootSignatureDesc(const CD3DX12_ROOT_PARAMETER1* rootParams, D3D12_ROOT_SIGNATURE_FLAGS flags)
	{
		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSigDesc;
		UINT numParams = sizeof(*rootParams) / sizeof(rootParams[0]);
		rootSigDesc.Init_1_1(numParams, rootParams, 0, nullptr, (D3D12_ROOT_SIGNATURE_FLAGS)flags);

		return rootSigDesc;
	}

	Ptr<ID3D12RootSignature> DxLayer::CreateRootSignature(Ptr<ID3D12Device2> device, const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* pDesc)
	{
		Ptr<ID3D12RootSignature> rootSignature = nullptr;

		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
		if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;

		ID3DBlob* pError = nullptr;
		ID3DBlob* rootSigBlob = nullptr;
		ThrowOnFail(D3DX12SerializeVersionedRootSignature(pDesc,
			featureData.HighestVersion, &rootSigBlob, &pError));

		ThrowOnFail(device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

		return rootSignature;
	}

	// Fundamentals:
	Ptr<IDXGIAdapter4> DxLayer::FindAdapter(bool useWarp)
	{
		Ptr<IDXGIFactory4> dxgiFac;
		UINT createFacFlags = 0;

#ifdef _DEBUG
		createFacFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		ThrowOnFail(CreateDXGIFactory2(createFacFlags, IID_PPV_ARGS(&dxgiFac)));

		Ptr<IDXGIAdapter1> dxgiAdap1 = nullptr;
		Ptr<IDXGIAdapter4> dxgiAdap4 = nullptr;

		if (useWarp)
		{
			ThrowOnFail(dxgiFac->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdap1)));
			dxgiAdap4 = (Ptr<IDXGIAdapter4>)dxgiAdap1;
		}
		else
		{
			size_t maxDedicatedVideoMem = 0;
			for (UINT i = 0; dxgiFac->EnumAdapters1(i, &dxgiAdap1) != DXGI_ERROR_NOT_FOUND; ++i)
			{
				DXGI_ADAPTER_DESC1 adapDesc;
				dxgiAdap1->GetDesc1(&adapDesc);

				// Find the compatible Adapter with the highest video memory
				if ((adapDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && // Adapter Software flag is not set...
					SUCCEEDED(D3D12CreateDevice(dxgiAdap1, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) && // Creating device succeeds...
					adapDesc.DedicatedVideoMemory > maxDedicatedVideoMem) // Adapter has higher video memory...
				{
					maxDedicatedVideoMem = adapDesc.DedicatedVideoMemory;
					dxgiAdap4 = (Ptr<IDXGIAdapter4>)dxgiAdap1;
				}
			}
		}

		return dxgiAdap4;
	}
	Ptr<ID3D12Device2> DxLayer::CreateDevice(Ptr<IDXGIAdapter4> adapter)
	{
		Ptr<ID3D12Device2> device;
		ThrowOnFail(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
		return device;
	}
	void DxLayer::InitInfoQueue()
	{
#ifdef _DEBUG
		Ptr<ID3D12InfoQueue> pInfoQueue = nullptr;
		if (SUCCEEDED(mpDevice->QueryInterface(&pInfoQueue)))
		{
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);

			//pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
		}
#endif
	}

	// Creating Resources:
	Ptr<ID3D12DescriptorHeap> DxLayer::CreateDescriptorHeap(Ptr<ID3D12Device> device, D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS flags)
	{
		Ptr<ID3D12DescriptorHeap> descHeap;
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = numDescriptors;
		desc.Type = type;
		desc.Flags = flags;

		ThrowOnFail(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descHeap)));
		return descHeap;
	}
	Ptr<ID3D12CommandQueue> DxLayer::CreateCommandQueue(Ptr<ID3D12Device2> pDevice, D3D12_COMMAND_LIST_TYPE type)
	{
		Ptr<ID3D12CommandQueue> q;
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		ThrowOnFail(pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&q)));
		return q;
	}
	Ptr<IDXGISwapChain4> DxLayer::CreateSwapChain(HWND wndHandle, Ptr<ID3D12CommandQueue> commandQueue, uint32_t w, uint32_t h, uint32_t bffCount)
	{
		// Create Factory:
		Ptr<IDXGIFactory4> dxgiFac;
		UINT createFacFlags = 0;
#ifdef _DEBUG
		createFacFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		ThrowOnFail(CreateDXGIFactory2(createFacFlags, IID_PPV_ARGS(&dxgiFac)));

		// Create SwpChain:
		Ptr<IDXGISwapChain4> dxgiSwapChain4;
		DXGI_SWAP_CHAIN_DESC1 desc = {};
		desc.Width = w;
		desc.Height = h;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Stereo = false;
		desc.SampleDesc = { 1, 0 };
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = bffCount;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0; // Allow tearing if supported...

		Ptr<IDXGISwapChain1> dxgiSwapChain1;
		ThrowOnFail(dxgiFac->CreateSwapChainForHwnd(
			commandQueue,
			wndHandle,
			&desc,
			nullptr,
			nullptr,
			&dxgiSwapChain1));

		ThrowOnFail(dxgiFac->MakeWindowAssociation(wndHandle, DXGI_MWA_NO_ALT_ENTER));
		dxgiSwapChain4 = (Ptr<IDXGISwapChain4>)dxgiSwapChain1;
		return dxgiSwapChain4;
	}
	Ptr<ID3D12CommandAllocator> DxLayer::CreateCommandAllocator(Ptr<ID3D12Device2> pDevice, D3D12_COMMAND_LIST_TYPE type)
	{
		Ptr<ID3D12CommandAllocator> cmdAlloc;
		ThrowOnFail(pDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&cmdAlloc)));

		return cmdAlloc;
	}
	Ptr<ID3D12GraphicsCommandList> DxLayer::CreateCommandList(Ptr<ID3D12Device2> device, Ptr<ID3D12CommandAllocator> alloc, D3D12_COMMAND_LIST_TYPE type)
	{
		Ptr<ID3D12GraphicsCommandList> cmdList;
		ThrowOnFail(device->CreateCommandList(0, type, alloc, nullptr, IID_PPV_ARGS(&cmdList)));
		ThrowOnFail(cmdList->Close());

		return cmdList;
	}

	Ptr<ID3D12PipelineState> DxLayer::CreatePipelineState(Ptr<ID3D12Device2> device, const D3D12_PIPELINE_STATE_STREAM_DESC& desc)
	{
		Ptr<ID3D12PipelineState> pipelineState;
		ThrowOnFail(device->CreatePipelineState(&desc, IID_PPV_ARGS(&pipelineState)));
		return pipelineState;
	}

	void DxLayer::LoadBufferResource(Ptr<ID3D12Device> device, Ptr<ID3D12GraphicsCommandList> cmdList, ID3D12Resource** destResource, ID3D12Resource** intResource, 
		size_t nElements, size_t elementSize, const void* pData, D3D12_RESOURCE_FLAGS flags)
	{
		size_t bufferSize = nElements * elementSize;

		auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize, flags);

		// Create Commited resource and put it in destResource
		ThrowOnFail(device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(destResource)));

		// Upload data to buffer:
		if (pData != nullptr)
		{
			// Create Commited resource (as uploadbuffer [HEAP_TYPE_UPLOAD && RESOURCE_STATE_GENERIC_READ]) 
			// To upload data to buffer:
			heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
			ThrowOnFail(device->CreateCommittedResource
			(
				&heapProps,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(intResource)
			));

			D3D12_SUBRESOURCE_DATA subResData = {};
			subResData.pData = pData;
			subResData.RowPitch = bufferSize;
			subResData.SlicePitch = bufferSize;

			UpdateSubresources(cmdList, *destResource, *intResource,
				0, 0, 1, &subResData);
		}
	}

	Ptr<ID3D12Fence> DxLayer::CreateFence(Ptr<ID3D12Device2> device)
	{
		Ptr<ID3D12Fence> f;
		ThrowOnFail(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&f)));

		return f;
	}
	HANDLE DxLayer::CreateFenceEventHandle()
	{
		HANDLE e;
		e = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(e && "Failed to create fence event...");

		return e;
	}

	uint64_t DxLayer::SignalFence(Ptr<ID3D12CommandQueue> cmdQueue, Ptr<ID3D12Fence> fence, uint64_t& fenceValue)
	{
		// The value the fence will be waiting for...
		uint64_t fenceValForSignal = ++fenceValue;

		// Tell the command queue to signal this fence with this value once it can...
		ThrowOnFail(cmdQueue->Signal(fence, fenceValForSignal));

		// This is the signal the fence will be waiting for...
		return fenceValForSignal;
	}

	void DxLayer::WaitForFenceValue(Ptr<ID3D12Fence> fence, uint64_t unlockValue, FenceEvent e, std::chrono::milliseconds duration)
	{
		// If my fence does not have the correct value yet... WAIT (for max 584 million years...)
		if (!IsFenceComplete(fence, unlockValue))
		{
			ThrowOnFail(fence->SetEventOnCompletion(unlockValue, e));
			::WaitForSingleObject(e, static_cast<DWORD>(duration.count()));
		}
	}

	bool DxLayer::IsFenceComplete(Ptr<ID3D12Fence> fence, uint64_t unlockValue)
	{
		return !(fence->GetCompletedValue() < unlockValue);
	}

	void DxLayer::Flush(Ptr<ID3D12CommandQueue> cmdQueue, Ptr<ID3D12Fence> fence, uint64_t& fenceValue, FenceEvent e)
	{
		uint64_t fenceUnlockValue = SignalFence(cmdQueue, fence, fenceValue);
		WaitForFenceValue(fence, fenceUnlockValue, e);
	}

	void DxLayer::Cmd_TransitionResource(Ptr<ID3D12GraphicsCommandList2> cmdList, Ptr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource, beforeState, afterState);
		cmdList->ResourceBarrier(1, &barrier);
	}

	// Misc:
	bool DxLayer::CheckTearingSupport()
	{
		bool allowTear = false;

		Ptr<IDXGIFactory4> fac4;
		if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&fac4))))
		{
			if (Ptr<IDXGIFactory5> fac5 = (Ptr<IDXGIFactory5>)fac4)
				allowTear = !FAILED(fac5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTear, sizeof(allowTear)));
		}

		return allowTear;
	}
	void DxLayer::EnableDebugLayer()
	{
#if defined _DEBUG
		ID3D12Debug* pDbgInt;
		ThrowOnFail(D3D12GetDebugInterface(IID_PPV_ARGS(&pDbgInt)));
		pDbgInt->EnableDebugLayer();
#endif
	}

#include <dxgidebug.h>
	void DxLayer::ReportLiveObjects()
	{
		IDXGIDebug1* dxgiDebug;
		DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug));

		//dxgiDebug->ReportLiveObjects(, DXGI_DEBUG_RLO_IGNORE_INTERNAL);
		dxgiDebug->Release();
	}
}





