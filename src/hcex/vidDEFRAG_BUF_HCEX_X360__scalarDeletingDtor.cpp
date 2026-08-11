#include "../headers/ws/vid/vidDEFRAG_BUF_HCEX_X360.h"

void operator delete(void *ptr); // boundary — CRT operator delete

// vidDEFRAG_BUF_HCEX_X360::`scalar deleting destructor' @ 0x823F18F0
// Compiler-generated deleting-destructor entry point: runs the destructor, then conditionally
// calls operator delete(this) when bit 0 of the flag argument is set.
vidDEFRAG_BUF_HCEX_X360 *vidDEFRAG_BUF_HCEX_X360::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~vidDEFRAG_BUF_HCEX_X360();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
