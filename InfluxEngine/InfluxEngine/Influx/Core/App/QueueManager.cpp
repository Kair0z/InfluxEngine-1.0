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

	sPtr<CommandQueue> QueueManager::GetQueue(const QueueTag queue) const
	{
		switch (queue)
		{
		case QueueTag::Compute: return mpComputeQueue;
		case QueueTag::Direct: return mpDirectQueue;
		case QueueTag::Upload: return mpUploadQueue;
		default: return mpDirectQueue;
		}
	}
	sPtr<CommandQueue> QueueManager::operator[](const QueueTag tag) const
	{
		return GetQueue(tag);
	}
}

