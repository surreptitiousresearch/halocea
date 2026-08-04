#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

/* apDLALLOC_IFACE::Malloc(uint) boundary wrapper (Saber ap platform layer, mangled
   ?Malloc@apDLALLOC_IFACE@@SAPAXI@Z) — returns void*, takes an unsigned byte count. */
void *apDLALLOC_IFACE_Malloc(unsigned int size);
void  dlDumpNoMemLeft(void);
/* ap memory-debug accounting (?__apMemoryDebugAdd@@YAHPAXIPBDIW4apMEM_BLOCK_TYPE@@@Z). boundary. */
int   __apMemoryDebugAdd(void *ptr, unsigned int size, const char *file, unsigned int line, apMEM_BLOCK_TYPE type);

void *dlMalloc(unsigned int size, const char *file, unsigned int line)
{
    unsigned int padded_size;
    void        *ptr;

    padded_size = size + 6;
    if (size == (unsigned int)-6)
        padded_size = 1;
    ptr = apDLALLOC_IFACE_Malloc(padded_size);
    if (!ptr)
        dlDumpNoMemLeft();
    __apMemoryDebugAdd(ptr, padded_size, file, line, AP_MEM_BT_ALLOCATOR);
    return ptr;
}
