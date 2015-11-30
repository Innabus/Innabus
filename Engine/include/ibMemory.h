#ifndef IB_MEMORY_H
#define IB_MEMORY_H

void* ibMemcpy(void* dst, void* src);
void* ibMemset(void* dst, u32 value, u32 size);

#include "ibMemory.inl"

#endif // IB_MEMORY_H
