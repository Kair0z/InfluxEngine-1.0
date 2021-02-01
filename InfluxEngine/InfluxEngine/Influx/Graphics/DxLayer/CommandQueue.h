#pragma once
#include <queue>

struct ID3D12Device2;
struct ID3D12CommandQueue;

/// <summary>
/// Wrapper class for D3D12 Command Queue
/// </summary>

namespace Influx
{
	class CommandQueue final
	{
	public:
		struct Desc { D3D12_COMMAND_LIST_TYPE type; };
		static sPtr<CommandQueue> Create(comPtr<ID3D12Device> device, const Desc& desc);
		~CommandQueue();

		// Returns a fresh command list in recording state [no need for resetting]
		Ptr<ID3D12GraphicsCommandList2> GetCommandList(comPtr<ID3D12Device> device);
		uint64_t ExecuteCommandList(Ptr<ID3D12GraphicsCommandList2> list);

		// Synchronization
		uint64_t Signal();
		bool IsFenceComplete(uint64_t unlockValue);
		void WaitForFence(uint64_t unlockValue);
		void Flush();

		comPtr<ID3D12CommandQueue> GetDxCommandQueue() const;

	private:
		CommandQueue() = default;
		IFX_DelCpyMove(CommandQueue);

		comPtr<ID3D12CommandQueue> mpDxCommandQueue;
		D3D12_COMMAND_LIST_TYPE mType;

		// Command Lists & Command Allocators:
		Ptr<ID3D12CommandAllocator> CreateCommandAllocator(comPtr<ID3D12Device> device);
		Ptr<ID3D12GraphicsCommandList2> CreateCommandList(comPtr<ID3D12Device> device, Ptr<ID3D12CommandAllocator> allocator);

		struct CommandAllocatorEntry // Keeping track of in-flight allocators
		{
			uint64_t fenceValue;
			Ptr<ID3D12CommandAllocator> commandAllocator;
		};
		using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
		using CommandListQueue = std::queue<Ptr<ID3D12GraphicsCommandList2>>;

		CommandAllocatorQueue mCommandAllocatorQueue;
		CommandListQueue mCommandListQueue;

		// Synchronization
		comPtr<ID3D12Fence> mpFence;
		HANDLE mFenceEvent;
		uint64_t mFenceVal;
	};
}


