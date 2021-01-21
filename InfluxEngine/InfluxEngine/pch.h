#pragma once

// Windows:
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma comment(lib, "user32.lib")

// DirectX 12:
#include <d3d12.h>
//#include <d3dx12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib,"d3dcompiler.lib")

// STL:
#include <stdint.h>
#include <cassert>
#include <algorithm>
#include <chrono>