#pragma once

// DLL Build
#ifdef IFLX_DLL
	#define IFLX_API __declspec(dllexport)
#else 
	#define IFLX_API __declspec(dllimport)
#endif


#include "Influx/Core/Entrypoint.h"
#include "Influx/Core/App/Application.h"
#include "Influx/Core/Alias.h"

// User available API
#include "Influx/Graphics/RenderPasses/Raster/RasterPass.h"
#include "Influx/Graphics/DxLayer/Fbo/FrameBuffer.h"