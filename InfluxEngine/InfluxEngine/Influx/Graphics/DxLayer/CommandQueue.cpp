#include "pch.h"
#include "CommandQueue.h"
#include "DxLayer.h"

namespace Influx
{
	sPtr<CommandQueue> CommandQueue::Create(comPtr<ID3D12Device> device, const Desc& desc)
	{
		sPtr<CommandQueue> commandQueue(new CommandQueue());

		// Create Dx Command Queue
		commandQueue->mpDxCommandQueue = 
			comPtr<ID3D12CommandQueue>(DxLayer::CreateCommandQueue(device.Get(), desc.type));

		commandQueue->mType = desc.type;

		// Create fence:
		commandQueue->mpFence = comPtr<ID3D12Fence>(DxLayer::CreateFence(device.Get()));
		commandQueue->mFenceEvent = DxLayer::CreateFenceEventHandle();
		commandQueue->mFenceVal = 0;

		return commandQueue;
	}


	// External usage of resources:
	uint64_t CommandQueue::ExecuteCommandList(Ptr<ID3D12GraphicsCommandList2> list)
	{
		// Close the list's command-recording state:
		ThrowOnFail(list->Close());

		// Get the allocator:
		Ptr<ID3D12CommandAllocator> cmdAlloc = nullptr;
		UINT dataSize = sizeof(ID3D12CommandAllocator);
		ThrowOnFail(list->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &cmdAlloc));

		Ptr<ID3D12CommandList> const ppCmdLists[] = { list };
		mpDxCommandQueue->ExecuteCommandLists(1, ppCmdLists);

		// Produce the fence release value to wait for...
		uint64_t fenceValue = Signal();

		// Add allocator to current queue (with accomagnied fencevalue to wait for...)
		mCommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, cmdAlloc });

		// Add List to current queue
		mCommandListQueue.push(list);

		// The ownership of the command allocator has been transferred to the ComPtr
		// in the command allocator queue. It is safe to release the reference 
		// in this temporary COM pointer here.
		cmdAlloc->Release();

		return fenceValue;
	}
	Ptr<ID3D12GraphicsCommandList2> CommandQueue::GetCommandList(comPtr<ID3D12Device> device)
	{
		Ptr<ID3D12CommandAllocator> allocator = nullptr;
		Ptr<ID3D12GraphicsCommandList2> cmdList = nullptr;

		// Command Lists can be reused always,
		// but command allocators cannot be reused whilst being 'in-flight'

		// Get useable allocator
		if (!mCommandAllocatorQueue.empty() && IsFenceComplete(mCommandAllocatorQueue.front().fenceValue))
		{
			// If the front allocator is finished...
			// Pop it off queue!
			allocator = mCommandAllocatorQueue.front().commandAllocator;
			mCommandAllocatorQueue.pop();
		}
		else allocator = CreateCommandAllocator(device);

		// Get useable list
		if (!mCommandListQueue.empty())
		{
			cmdList = mCommandListQueue.front();
			mCommandListQueue.pop();
		}
		else cmdList = CreateCommandList(device, allocator);

		ThrowOnFail(allocator->Reset());
		ThrowOnFail(cmdList->Reset(allocator, nullptr));

		// Couple the allocator to the list and ship them off as a couple :D
		ThrowOnFail(cmdList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), allocator));
		return cmdList;
	}

	// Internal creation of resources:
	Ptr<ID3D12CommandAllocator> CommandQueue::CreateCommandAllocator(comPtr<ID3D12Device> device)
	{
		return DxLayer::CreateCommandAllocator(device.Get(), mType);
	}
	Ptr<ID3D12GraphicsCommandList2> CommandQueue::CreateCommandList(comPtr<ID3D12Device> device, Ptr<ID3D12CommandAllocator> allocator)
	{
		// TODO: Secure compatibility list1 vs list2...
		Ptr<ID3D12GraphicsCommandList2> l2 = nullptr;
		DxLayer::CreateCommandList(device.Get(), allocator, mType)->QueryInterface(&l2);
		return l2;
	}

#pragma region Synchronization
	uint64_t CommandQueue::Signal()
	{
		return DxLayer::SignalFence(mpDxCommandQueue.Get(), mpFence.Get(), mFenceVal);
	}
	bool CommandQueue::IsFenceComplete(uint64_t unlockValue)
	{
		return DxLayer::IsFenceComplete(mpFence.Get(), unlockValue);
	}
	void CommandQueue::WaitForFence(uint64_t unlockValue)
	{
		DxLayer::WaitForFenceValue(mpFence.Get(), unlockValue, mFenceEvent);
	}
	void CommandQueue::Flush()
	{
		DxLayer::FlushCommandQueue(mpDxCommandQueue.Get(), mpFence.Get(), mFenceVal, mFenceEvent);
	}
#pragma endregion

	comPtr<ID3D12CommandQueue> CommandQueue::GetDxCommandQueue() const
	{
		return mpDxCommandQueue;
	}

	CommandQueue::~CommandQueue()
	{
		mpFence->Release();
		mpDxCommandQueue->Release();
	}
}

