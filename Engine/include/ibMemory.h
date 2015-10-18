#ifndef IB_MEMORY_H
#define IB_MEMORY_H

void* ibMemcpy(void* dst, void* src);
void* ibMemset(void* dst, u32 size, u32 value);

#include "ibMemory.inl"

#endif // IB_MEMORY_H
