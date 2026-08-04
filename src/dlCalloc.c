#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

void *apDLALLOC_IFACE_Calloc(unsigned int size);
void  dlDumpNoMemLeft(void);
int   __apMemoryDebugAdd(void *ptr, unsigned int size, const char *file, unsigned int line, apMEM_BLOCK_TYPE type);

void *dlCalloc(unsigned int num, unsigned int size, const char *file, unsigned int line)
{
    unsigned int padded_size;
    void        *ptr;

    padded_size = num * size + 6;
    ptr = apDLALLOC_IFACE_Calloc(padded_size);
    if (!ptr)
        dlDumpNoMemLeft();
    __apMemoryDebugAdd(ptr, padded_size, file, line, AP_MEM_BT_ALLOCATOR);
    return ptr;
}
