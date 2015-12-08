#include "ibRenderDevice.h"

#include "ibLog.h"

#include "DXGI.h"

void LogAdapterDesc(DXGI_ADAPTER_DESC1* pDesc, u32 idx)
{
		ibLog("[RENDER] Adapter %u:\n"
			  "[RENDER]\tDesc: %S\n"
			  "[RENDER]\tVendorId: %u\n"
			  "[RENDER]\tDeviceId: %u\n"
			  "[RENDER]\tSubSysId: %u\n"
			  "[RENDER]\tRevision: %u\n"
			  "[RENDER]\tDedicated video memory: %u\n"
			  "[RENDER]\tDedicated system memory: %u\n"
			  "[RENDER]\tShared system memory: %u\n"
			  "[RENDER]\tLUID: %X-%X\n"
			  "[RENDER]\tFlags: %u\n",
			  idx, pDesc->Description, pDesc->VendorId,
			  pDesc->DeviceId, pDesc->SubSysId, pDesc->Revision,
			  pDesc->DedicatedVideoMemory, pDesc->DedicatedSystemMemory,
			  pDesc->SharedSystemMemory, 
			  pDesc->AdapterLuid.HighPart, pDesc->AdapterLuid.LowPart,
			  pDesc->Flags);
}

void LogOutputDesc(DXGI_OUTPUT_DESC* pDesc, int idx)
{
	ibLog("[RENDER]\t\tOutput %u:\n"
		"[RENDER]\t\t\tName: %S\n"
		"[RENDER]\t\t\tCoords:\n"
		"[RENDER]\t\t\t\t{ top: %d, left: %d, bottom: %d, right: %d }\n"
		"[RENDER]\t\t\tAttached to desktop: %s\n"
		"[RENDER]\t\t\tRotation: %u\n",
		idx, pDesc->DeviceName,  
		pDesc->DesktopCoordinates.top, pDesc->DesktopCoordinates.left,
		pDesc->DesktopCoordinates.bottom, pDesc->DesktopCoordinates.right,
		pDesc->AttachedToDesktop ? "TRUE" : "FALSE", pDesc->Rotation);
}

void ibRenderDevice::PlatformInit()
{
	ComPtr<IDXGIFactory1> pFactory;
	CreateDXGIFactory1(__uuidof(IDXGIFactory1), pFactory);
	ibAssert(*pFactory);
	u32 i = 0;
	ComPtr<IDXGIAdapter1> pAdapter;
	while (pFactory->EnumAdapters1(i, pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 Desc;
		pAdapter->GetDesc1(&Desc);
		LogAdapterDesc(&Desc, i);
		
		u32 n = 0;
		ComPtr<IDXGIOutput> pOutput;
		while (pAdapter->EnumOutputs(n, pOutput) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_OUTPUT_DESC outputDesc;
			pOutput->GetDesc(&outputDesc);
			LogOutputDesc(&outputDesc, n);

			++n;
			pOutput->Release();
		}

		++i;
		pAdapter->Release();
		ibLog("[RENDER] --------------------------------------------------------------------\n");
	}
}
