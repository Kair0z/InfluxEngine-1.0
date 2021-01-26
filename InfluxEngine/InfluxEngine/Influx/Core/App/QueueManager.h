#pragma once
namespace Influx
{
	class CommandQueue;
	class QueueManager final
	{
	public:
		static sPtr<QueueManager> CreateQueues(comPtr<ID3D12Device> device);

		enum class QueueTag { Direct, Upload, Compute };
		sPtr<CommandQueue> GetQueue(const QueueTag queue) const;
		sPtr<CommandQueue> operator[](const QueueTag queue) const;

	private:
		QueueManager() = default;

		sPtr<CommandQueue> mpDirectQueue;
		sPtr<CommandQueue> mpUploadQueue;
		sPtr<CommandQueue> mpComputeQueue;
	};
}


