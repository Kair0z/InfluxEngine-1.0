#include "pch.h"
#include "QueueManager.h"
#include "Influx/Graphics/DxLayer/CommandQueue.h"

namespace Influx
{
	sPtr<QueueManager> QueueManager::CreateQueues(comPtr<ID3D12Device> device)
	{
		sPtr<QueueManager> queueManager(new QueueManager());

		// Direct Queue:
		queueManager->mpDirectQueue = CommandQueue::Create(device, { D3D12_COMMAND_LIST_TYPE_DIRECT });

		// Upload Queue:
		queueManager->mpUploadQueue = CommandQueue::Create(device, { D3D12_COMMAND_LIST_TYPE_COPY });

		// Compute Queue:
		queueManager->mpComputeQueue = CommandQueue::Create(device, { D3D12_COMMAND_LIST_TYPE_COMPUTE });

		return queueManager;
	}

	sPtr<CommandQueue> QueueManager::GetQueue(const QType queue) const
	{
		switch (queue)
		{
		case QType::Compute: return mpComputeQueue;
		case QType::Direct: return mpDirectQueue;
		case QType::Upload: return mpUploadQueue;
		default: return mpDirectQueue;
		}
	}
	sPtr<CommandQueue> QueueManager::operator[](const QType tag) const
	{
		return GetQueue(tag);
	}
}

