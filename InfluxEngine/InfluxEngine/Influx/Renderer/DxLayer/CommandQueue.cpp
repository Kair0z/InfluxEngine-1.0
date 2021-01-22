#include "pch.h"
#include "CommandQueue.h"
#include "DxLayer.h"

namespace Influx
{
	sPtr<CommandQueue> CommandQueue::Create(sPtr<ID3D12Device2> device, const Desc& desc)
	{
		sPtr<CommandQueue> commandQueue(new CommandQueue());

		// Create Dx Command Queue
		commandQueue->mpDxCommandQueue = 
			sPtr<ID3D12CommandQueue>(DxLayer::CreateCommandQueue(device.get(), desc.type));

		commandQueue->mType = desc.type;

		// Create fence:
		commandQueue->mpFence = sPtr<ID3D12Fence>(DxLayer::CreateFence(device.get()));
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

		return fenceValue;
	}
	Ptr<ID3D12GraphicsCommandList2> CommandQueue::GetCommandList(sPtr<ID3D12Device2> device)
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
	Ptr<ID3D12CommandAllocator> CommandQueue::CreateCommandAllocator(sPtr<ID3D12Device2> device)
	{
		return DxLayer::CreateCommandAllocator(device.get(), mType);
	}
	Ptr<ID3D12GraphicsCommandList2> CommandQueue::CreateCommandList(sPtr<ID3D12Device2> device, Ptr<ID3D12CommandAllocator> allocator)
	{
		// TODO: Secure compatibility list1 vs list2...
		Ptr<ID3D12GraphicsCommandList2> l2 = nullptr;
		DxLayer::CreateCommandList(device.get(), allocator, mType)->QueryInterface(&l2);
		return l2;
	}

#pragma region Synchronization
	uint64_t CommandQueue::Signal()
	{
		return DxLayer::SignalFence(mpDxCommandQueue.get(), mpFence.get(), mFenceVal);
	}
	bool CommandQueue::IsFenceComplete(uint64_t unlockValue)
	{
		return DxLayer::IsFenceComplete(mpFence.get(), unlockValue);
	}
	void CommandQueue::WaitForFence(uint64_t unlockValue)
	{
		DxLayer::WaitForFenceValue(mpFence.get(), unlockValue, mFenceEvent);
	}
	void CommandQueue::Flush()
	{
		DxLayer::Flush(mpDxCommandQueue.get(), mpFence.get(), mFenceVal, mFenceEvent);
	}
#pragma endregion

	sPtr<ID3D12CommandQueue> CommandQueue::GetDxCommandQueue() const
	{
		return mpDxCommandQueue;
	}

	CommandQueue::~CommandQueue()
	{
		mpFence->Release();
		mpDxCommandQueue->Release();
	}
}
