#include <stdint.h>
#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

void *apDLALLOC_IFACE_Memalign(uint32_t size, uint32_t align);
void  dlDumpNoMemLeft(void);
int   __apMemoryDebugAdd(void *ptr, uint32_t size, const char *file, uint32_t line, apMEM_BLOCK_TYPE type);

void *dlMemalign(uint32_t size, uint32_t align, const char *file, uint32_t line)
{
    uint32_t padded_size;
    void    *ptr;

    padded_size = size + 6;
    if (size == (uint32_t)-6)
        padded_size = 1;
    ptr = apDLALLOC_IFACE_Memalign(padded_size, align);
    if (!ptr)
        dlDumpNoMemLeft();
    __apMemoryDebugAdd(ptr, padded_size, file, line, AP_MEM_BT_ALLOCATOR);
    return ptr;
}
