#ifndef IB_ENGINE_SETTINGS_H
#define IB_ENGINE_SETTINGS_H
#pragma once

struct ibEngineSettings
{
	static void Serialize(const ibEngineSettings& settings,
		                  char** ppOut,
						  bool writeProtectedMembers = false
	);

	static void Deserialize(ibEngineSettings* pSettings,
		                    const char* pInput,
		                    bool readProtectedMembers = false
	);

	// Display Adapter, default 0 -- use the default adapter
	int Display;

	// DisplayMode, 0 = windowed, 1 = full screen, 2 = window borderless
	int DisplayMode;

	// Custom resolution, if using a fixed sized render target before post this
	// only affects the display render size (window client size, on windows)
	int DisplayWidth;
	int DisplayHeight;

	//////////////////////////////////////////////////////////////////////////////
	// Protected members
	//////////////////////////////////////////////////////////////////////////////

	// UseWindow defaults to true (1). If this member is false no window is shown
	// and the renderer is not initialized automatically.
	int UseWindow;
};

#endif // IB_ENGINE_SETTINGS_H
