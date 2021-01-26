#include "pch.h"
#include "Window.h"
#include "Influx/Graphics/DxLayer/SwapChain.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


//Windows GUI global Variables & functions
HMENU hMenu;
void CreateMenus(HWND hWnd);

//Windows GUI ID's of the Header menu
#define IFX_LOADMENU_LOADSCENE 1
#define IFX_LOADMENU_LOADOBJECT 2
#define IFX_LOADMENU_LOADSHADER 3
#define IFX_EDITMENU_ANTIALIASING 4
#define IFX_EDITMENU_RENDERINGTECHNIQUE_RASTERIZER 5
#define IFX_EDITMENU_RENDERINGTECHNIQUE_RAYTRACER 6
#define IFX_EDITMENU_POSTPROCESSING 7
#define IFX_HELPMENU 8 

// Windows Callback function:
LRESULT CALLBACK WndProc(HWND hwnd, UINT mssg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, mssg, wParam, lParam))
		return true;

	switch (mssg)
	{
	case WM_CREATE:
		CreateMenus(hwnd);
		break;
	case WM_COMMAND:

		//When button is pressed in the Windows GIU [TODO]
		switch (lParam)
		{
		case IFX_LOADMENU_LOADSCENE:
			break;
		case IFX_LOADMENU_LOADOBJECT:
			break;
		case IFX_LOADMENU_LOADSHADER:
			break;
		}

		break;
	case WM_SYSKEYDOWN:
		break;

	case WM_KEYDOWN:
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProcW(hwnd, mssg, wParam, lParam);
	}

	return 0;
}

void CreateMenus(HWND hWnd)
{

	//Create Header Menu Bar
	hMenu = CreateMenu();

	//Create Load menu and sub sections
	HMENU hLoadMenu = CreateMenu();

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hLoadMenu, (LPCWSTR)L"Load");
	{
		AppendMenu(hLoadMenu, MF_STRING, IFX_LOADMENU_LOADSCENE, (LPCWSTR)L"Load Scene");
		AppendMenu(hLoadMenu, MF_STRING, IFX_LOADMENU_LOADOBJECT, (LPCWSTR)L"Load Object");
		AppendMenu(hLoadMenu, MF_STRING, IFX_LOADMENU_LOADSHADER, (LPCWSTR)L"Load Shader");
	}

	//Edit menu and other sub sections
	HMENU hEditMenu = CreateMenu();
	
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, (LPCWSTR)L"Edit");
	{
		//Anti-Aliasing modes
		AppendMenu(hEditMenu, MF_STRING, 4, (LPCWSTR)L"Anti Aliasing");
		
		//Techniques
		HMENU hEditMenuRenderingTechnique = CreateMenu();

		AppendMenu(hEditMenu, MF_POPUP, (UINT_PTR)hEditMenuRenderingTechnique, (LPCWSTR)L"Rendering Technique");
		{
			AppendMenu(hEditMenuRenderingTechnique, MF_STRING, IFX_EDITMENU_RENDERINGTECHNIQUE_RASTERIZER, (LPCWSTR)L"Rasterizer Technique");
			AppendMenu(hEditMenuRenderingTechnique, MF_STRING, IFX_EDITMENU_RENDERINGTECHNIQUE_RAYTRACER, (LPCWSTR)L"Raytracer Technique");
		}

		//Post Processing Modes
		AppendMenu(hEditMenu, MF_STRING, IFX_EDITMENU_POSTPROCESSING, (LPCWSTR)L"Post Processing");
	}

	//Help menu [TODO]
	AppendMenu(hMenu, MF_STRING, IFX_HELPMENU, (LPCWSTR)L"Help");

	//Final step: to set the menu
	SetMenu(hWnd, hMenu);
}

namespace Influx
{
	sPtr<Window> Window::Create(const Desc& wDesc, HINSTANCE hInst)
	{
		sPtr<Window> pWindow = sPtr<Window>(new Window());
		pWindow->mWindowDesc = wDesc;

		// Register window class:
		const wchar_t* wClass = L"DX12WindowClass";
		RegisterWindowClass(hInst, wClass);

		// Create Windows window:
		pWindow->mWindowHandle = CreateWindow(hInst, wDesc.name, wClass, wDesc.dimensions.x, wDesc.dimensions.y);
		SetWindowIcon("../../InfluxEngine/Resources/Main/Logo.ico", pWindow->mWindowHandle);

		return pWindow;
	}

	void Window::Show()
	{
		::ShowWindow(mWindowHandle, SW_SHOW);
	}

	HWND Window::GetWindowsHandle() const
	{
		return mWindowHandle;
	}

	Window::Desc Window::GetWindowsDesc() const
	{
		return mWindowDesc;
	}

	sPtr<SwapChain> Window::SetupSwapChain(comPtr<ID3D12Device> device, const uint8_t bufferCount)
	{
		// Create Swapchain:
		SwapChain::Desc swapDesc{};
		swapDesc.bufferCount = bufferCount;
		swapDesc.dimensions = mWindowDesc.dimensions;
		swapDesc.windowHandle = mWindowHandle;

		mpSwapChain = SwapChain::Create(swapDesc, nullptr, device);
		return mpSwapChain;
	}

	void Window::SetWindowIcon(const std::string& iconPath, const HWND& windowHandle)
	{
		HANDLE hIcon = LoadImageA(GetModuleHandle(NULL), iconPath.c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		HWND hWnd = windowHandle ? windowHandle : GetActiveWindow();
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		
	}

	void Window::RegisterWindowClass(HINSTANCE i, const wchar_t* wndClassName)
	{
		WNDCLASSEXW windowClass = {};
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = &WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = i;
		windowClass.hIcon = ::LoadIcon(i, NULL);
		windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = wndClassName;
		windowClass.hIconSm = ::LoadIcon(i, NULL);

		static ATOM atom = ::RegisterClassExW(&windowClass);
		assert(atom > 0);
	}

	HWND Window::CreateWindow(HINSTANCE i, const wchar_t* title, const wchar_t* wndClassName, uint32_t w, uint32_t h)
	{
		// Setup Window Rect
		int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

		RECT wndRect = { 0, 0, (LONG)w, (LONG)h };
		::AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, FALSE);

		int windowWidth = wndRect.right - wndRect.left;
		int windowHeight = wndRect.bottom - wndRect.top;
		int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
		int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);

		// Create Window:
		HWND hWnd = ::CreateWindowExW(
			NULL,
			wndClassName,
			title,
			WS_OVERLAPPEDWINDOW,
			windowX,
			windowY,
			windowWidth,
			windowHeight,
			NULL,
			NULL,
			i,
			nullptr
		);

		assert(hWnd && "Failed to Create Window...");

		return hWnd;
	}
}

