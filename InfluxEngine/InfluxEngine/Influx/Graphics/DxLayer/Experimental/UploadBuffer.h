#pragma once
#include <deque>

namespace Influx
{
	class UploadBuffer
	{
	public:
		struct Allocation
		{
			void* cpu;
			D3D12_GPU_VIRTUAL_ADDRESS gpu;
		};

		explicit UploadBuffer(const size_t pageSize = 1000000); // ~2MB
		size_t GetPageSize() const { return mPageSize; }

		// Allocate a chunk of memory inside the linear allocator
		Allocation Allocate(size_t nBytes, size_t align);

		// Release all allocated pages (Only when commandqueue is finished...)
		void Reset();

	private:
		struct Page
		{
			Page(size_t nBytes);
			bool HasSpace(size_t nBytes, size_t align) const;
			Allocation Allocate(size_t nBytes, size_t align);
			void Reset();

			// Base Pointer:
			void* mpCpuBase;
			D3D12_GPU_VIRTUAL_ADDRESS mGpuBase;

			size_t mPageSize;
			size_t mCurrentOffset; // current allocation offset in bytes...
		};

		comPtr<ID3D12Resource> mpDxResource;

		using PagePool = std::deque<sPtr<Page>>;
		sPtr<Page> GetPage(); // Get free page, or create a new one...

		PagePool mPagePool;
		PagePool mAvPages;

		sPtr<Page> mCurrentPage;
		size_t mPageSize;
	};
}


