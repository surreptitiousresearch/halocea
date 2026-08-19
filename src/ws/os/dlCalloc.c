/* dlCalloc @0x82506688
 *
 * DEVIATION: this TU is C++, not C (see dlMalloc.c) — a FLAT export whose callees are all mangled. */

#include "headers/ws/ap/apDLALLOC_IFACE.h"
#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

void dlDumpNoMemLeft(void); /* ?dlDumpNoMemLeft@@YAXXZ */
int  __apMemoryDebugAdd(void *ptr, unsigned int size, const char *file, unsigned int line,
                        apMEM_BLOCK_TYPE type);

extern "C" void *dlCalloc(unsigned int num, unsigned int size, const char *file, unsigned int line)
{
    unsigned int padded_size;
    void        *ptr;

    padded_size = num * size + 6;
    ptr = apDLALLOC_IFACE::Calloc(padded_size);
    if (!ptr)
        dlDumpNoMemLeft();
    __apMemoryDebugAdd(ptr, padded_size, file, line, AP_MEM_BT_ALLOCATOR);
    return ptr;
}
