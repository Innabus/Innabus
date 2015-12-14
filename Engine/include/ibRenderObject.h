#ifndef IB_RENDER_OBJECT_H
#define IB_RENDER_OBJECT_H
#pragma once

class IB_RENDER_EXPORT ibRenderObject
{
public:
	virtual ~ibRenderObject() {}
	virtual void Render() = 0;
};

#endif // IB_RENDER_OBJECT_H
