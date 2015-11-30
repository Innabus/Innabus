#include <memory.h>

inline
void* ibMemcpy(void* dst, const void* src, u32 size)
{
	memcpy(dst, src, size);
	return dst;
}

inline
void* ibMemset(void* dst, u32 value, u32 size)
{
	memset(dst, value, size);
	return dst;
}
