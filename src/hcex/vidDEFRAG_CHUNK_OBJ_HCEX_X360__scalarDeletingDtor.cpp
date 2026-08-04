#include "../headers/ws/vid/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"

void operator delete(void *ptr); // boundary — CRT operator delete

// vidDEFRAG_CHUNK_OBJ_HCEX_X360::`scalar deleting destructor' @ 0x823F131C
// Compiler-generated deleting-destructor entry point: runs the destructor, then conditionally
// calls operator delete(this) when bit 0 of the flag argument is set.
vidDEFRAG_CHUNK_OBJ_HCEX_X360 *vidDEFRAG_CHUNK_OBJ_HCEX_X360::scalarDeletingDtor(unsigned char deleteFlags)
{
    this->~vidDEFRAG_CHUNK_OBJ_HCEX_X360();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
