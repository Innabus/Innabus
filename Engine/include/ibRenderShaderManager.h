#ifndef IB_SHADER_MANAGER_H
#define IB_SHADER_MANAGER_H
#pragma once

#include "ibRenderDefs.h"
#include "ibRenderShader.h"

#include "ibStd.h"

class IB_RENDER_EXPORT ibShaderManager
{
public:
	static ibShaderManager* Get();
	static void Destroy();

	u32 LoadShader(const char* pName, void* pData, u32 len, ibShaderType type);
	u32 LoadShaderFromFile(const char* pFilename, ibShaderType type);
	
	void UnloadShader(u32);
	void UnloadShader(const char* pName);

	ibRenderShader GetShader(u32 id);
	ibRenderShader GetShader(const char* pName);

private:
	ibShaderManager();
	~ibShaderManager();

	ibMap<u32, ibRenderShader> m_shaderMap;
};

#define g_shaderManager (*ibShaderManager::Get())

#endif // IB_SHADER_MANAGER_H
