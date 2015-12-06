#include "ibEngineSettings.h"

#include "ibStd.h"
#include "ibUtil.h"

#include <algorithm>
#include <functional>
#include <cctype>

void ibEngineSettings::Serialize(const ibEngineSettings& settings, char** ppOut, bool writeProtectedMembers)
{
	ibAssert(ppOut);

	ibStringStream stream;
	stream << "Display=" << settings.Display
		 << "\nDisplayMode=" << settings.DisplayMode
		 << "\nDisplayWidth=" << settings.DisplayWidth
		 << "\nDisplayHeight=" << settings.DisplayHeight;

	if (writeProtectedMembers)
	{
		stream << "UseWindow=" << settings.UseWindow << "\n";
	}

	u32 size = stream.str().length();
	char* out = new char[size + 1];
	ibAssert(out);
	strcpy(out, stream.str().c_str());

	*ppOut = out;
}

#pragma warning(push)
#pragma warning(disable : 4996)
void ibEngineSettings::Deserialize(ibEngineSettings* pSettings, const char* pInput, bool readProtectedMembers)
{
	struct settingPair {
		const char* key;
		int* out;
		bool read;
	} map [] = {
		{ "Display",       &pSettings->Display,       true },
		{ "DisplayMode",   &pSettings->DisplayMode,   true },
		{ "DisplayWidth",  &pSettings->DisplayWidth,  true },
		{ "DisplayHeight", &pSettings->DisplayHeight, true },
		{ "UseWindow",     &pSettings->UseWindow,     readProtectedMembers },
	};

	const char* p = pInput;
	const char* s = p;
	while (*p)
	{
		if (*p++ == '\n')
		{
			const char* front = std::find_if_not(s, p, std::isspace);
			const char* sep = std::find(front, p, '=');
			const char* back = ibMax(sep, front);
			while (back > front && isspace(*(back - 1)))
				--back;
			ibString key(front, back);
			front = std::find_if_not(ibMin(sep + 1, p), p, std::isspace);
			back = ibMax(p, front);
			while (back > front && isspace(*(back - 1)))
				--back;
			ibString val(front, back);

			if (key.size() && val.size())
			{
				
				for (u32 n = 0; n < sizeof(map) / sizeof(settingPair); ++n)
				{
					// Use proper keys, and do not retain if invalid
					if (!stricmp(key.c_str(), map[n].key) && map[n].read)
						*map[n].out = atoi(val.c_str());
				}
			}

			s = p;
		}
	}
}
#pragma warning(pop)
