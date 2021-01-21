#pragma once

// Windows:
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma comment(lib, "user32.lib")

// DirectX 12:
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma warning( push )
#pragma warning( disable : 4324 )
#include <d3dx12/d3dx12.h>
#pragma warning( pop )

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

// STL:
#include <stdint.h>
#include <cassert>
#include <algorithm>
#include <chrono>

inline void ThrowOnFail(HRESULT hr)
{
	if (FAILED(hr)) 
		throw std::exception();
}