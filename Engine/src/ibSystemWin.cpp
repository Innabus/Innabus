#include <Windows.h>
#include <tchar.h>
#include "ibSystem.h"

#include "ibEngineSettings.h"
#include "ibMCP.h"
#include "ibUtil.h"

#include "ibMemory.h"

namespace
{
	void* s_systemHeapPtr = 0;

	ibOS::Window s_mainWnd;

	LRESULT CALLBACK ibWndProc(HWND, UINT, WPARAM, LPARAM);

	void WindowCreate()
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.lpfnWndProc = ibWndProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.lpszClassName = _T("Innabus Main Window");
		RegisterClassEx(&wc);

		DWORD width = g_engineSettings.DisplayWidth;
		DWORD height = g_engineSettings.DisplayHeight;
		DWORD dwStyle = (WS_OVERLAPPEDWINDOW | WS_VISIBLE) ^ WS_THICKFRAME;

		RECT rect = { 0 };
		rect.right = width;
		rect.bottom = height;
		AdjustWindowRect(&rect, dwStyle, FALSE);

		s_mainWnd = CreateWindow(_T("Innabus main Window"), _T("Innabus"), dwStyle,
			                     CW_USEDEFAULT, CW_USEDEFAULT, 
								 width ? rect.right - rect.left : CW_USEDEFAULT, 
								 height ? rect.bottom - rect.top : CW_USEDEFAULT,
								 NULL, NULL, GetModuleHandle(NULL), NULL);
	}

	LRESULT CALLBACK ibWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void ibSystem::Init()
{
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hrsrc = FindResource(hModule, _T("EngineSettings"), RT_RCDATA);
	if (hrsrc)
	{
		DWORD dwSize = SizeofResource(hModule, hrsrc);
		HGLOBAL hSettings = LoadResource(hModule, hrsrc);
		if (hSettings)
		{
			char* pSettings = (char*)LockResource(hSettings);
			char* pSafe = new char[dwSize + 1];
			pSafe[dwSize] = 0;
			ibMemcpy(pSafe, pSettings, dwSize);
			ibEngineSettings::Deserialize(GetSettings(), pSafe, true);
			delete [] pSafe;
		}
	}
	SetLastError(0); // Don't care why it failed, only that it did

	if (g_engineSettings.UseWindow)
		WindowCreate();
}

int ibSystem::RunLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	g_mcp.Shutdown();
	return msg.wParam;
}

void* ibSystem::SystemAlloc(u32 sizeBytes)
{
	ibAssertMsg(s_systemHeapPtr == 0, "Attempt to allocate system heap twice.");
	s_systemHeapPtr = HeapAlloc(GetProcessHeap(), 0, sizeBytes);
	ibAssertMsg(s_systemHeapPtr, "Failed to allocate system heap.");
	return s_systemHeapPtr;
}

ibOS::Window ibSystem::GetMainWindow()
{
	return s_mainWnd;
}
