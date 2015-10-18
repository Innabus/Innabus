#include <memory.h>

inline
void* ibMemcpy(void* dst, const void* src, u32 size)
{
	memcpy(dst, src, size);
	return dst;
}

inline
void* ibMemset(void* dst, u32 size, u32 value)
{
	memset(dst, value, size);
	return dst;
}
