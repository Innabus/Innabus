#ifndef IB_RENDER_OBJECT_H
#define IB_RENDER_OBJECT_H
#pragma once

class ibRenderCommandList;

class IB_RENDER_EXPORT ibRenderObject
{
public:
	virtual ~ibRenderObject() {}
	virtual void Render(ibRenderCommandList& commandList) = 0;
};

#endif // IB_RENDER_OBJECT_H
