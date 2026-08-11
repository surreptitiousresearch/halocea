#include "../headers/ws/vid/vidDEFRAG_POOL_HCEX_X360.h"

void operator delete(void *ptr); // boundary — CRT operator delete

// vidDEFRAG_POOL_HCEX_X360::`vector deleting destructor' @ 0x823F1478
// Compiler-generated deleting-destructor entry point. DB shows the (this, flags) shape only (no
// array-count parameter, despite the `??_E` vector-deleting mangle) — reproduced as-is.
vidDEFRAG_POOL_HCEX_X360 *vidDEFRAG_POOL_HCEX_X360::vectorDeletingDtor(unsigned char deleteFlags)
{
    this->~vidDEFRAG_POOL_HCEX_X360();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
