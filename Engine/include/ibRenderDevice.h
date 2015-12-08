#ifndef IB_RENDER_DEVICE_H
#define IB_RENDER_DEVICE_H
#pragma once

class IB_RENDER_EXPORT ibRenderDevice
{
public:
	ibRenderDevice();
	~ibRenderDevice();

private:
	void PlatformInit();
};

#endif // IB_RENDER_DEVICE_H
