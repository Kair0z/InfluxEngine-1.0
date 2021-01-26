#include "pch.h"
#include "UploadBuffer.h"

namespace Influx
{
	UploadBuffer::UploadBuffer(const size_t pageSize) : mPageSize{pageSize}{}

	UploadBuffer::Allocation UploadBuffer::Allocate(size_t nBytes, size_t align)
	{
		if (nBytes > mPageSize) throw std::bad_alloc();

		// If no current page yet, or not enough space in current page...
		// Create new page!
		if (!mCurrentPage || mCurrentPage->HasSpace(nBytes, align))
			mCurrentPage = GetPage();

		return mCurrentPage->Allocate(nBytes, align);
	}

	void UploadBuffer::Reset()
	{
		mCurrentPage = nullptr;
		mAvPages = mPagePool;
		for (auto page : mAvPages)
			page->Reset();
	}

	sPtr<UploadBuffer::Page> UploadBuffer::GetPage()
	{
		sPtr<Page> page;

		if (!mAvPages.empty()) // use an available page...
		{
			page = mAvPages.front();
			mAvPages.pop_front();
		}
		else // Create new page
		{
			page = std::make_shared<Page>(mPageSize);
			mPagePool.push_back(page);
		}

		return page;
	}





	UploadBuffer::Page::Page(size_t nBytes): 
		mPageSize(nBytes),
		mCurrentOffset(0),
		mpCpuBase(nullptr),
		mGpuBase(D3D12_GPU_VIRTUAL_ADDRESS(0))
	{
		// Get Device
	}

}

