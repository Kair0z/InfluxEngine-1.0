#pragma once
#include <memory>

namespace Influx
{
	template <typename T> using Ptr = T*;
	template <typename T> using sPtr = std::shared_ptr<T>;
	template <typename T> using uPtr = std::unique_ptr<T>;
	template <typename T> using comPtr = Microsoft::WRL::ComPtr<T>;

	using ui64 = uint64_t;
	using ui32 = uint32_t;
}