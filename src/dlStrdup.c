/* dlStrdup @0x82506760
 *
 * DEVIATION: this TU is C++, not C (see dlMalloc.c) — a FLAT export whose callees are all mangled. */

#include <stdint.h>
#include <string.h>
#include "headers/ws/ap/apDLALLOC_IFACE.h"
#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

void dlDumpNoMemLeft(void); /* ?dlDumpNoMemLeft@@YAXXZ */
int  __apMemoryDebugAdd(void *ptr, uint32_t size, const char *file, uint32_t line,
                        apMEM_BLOCK_TYPE type);

extern "C" char *dlStrdup(char *ptr, const char *file, uint32_t line)
{
    const char *src;
    char       *scan;
    uint32_t    padded_size;
    void       *dup;

    if (!ptr)
        return ptr;

    src = ptr;
    scan = ptr;
    while (*scan++)
        ;
    /* length + terminator (scan - ptr == strlen + 1), padded by allocator overhead (+6),
     * with the length underflow guard mirroring dlMalloc. */
    padded_size = (uint32_t)(scan - ptr) - 1 + 7;
    if ((uint32_t)(scan - ptr) == (uint32_t)-6)
        padded_size = 1;
    dup = apDLALLOC_IFACE::Malloc(padded_size);
    if (!dup)
        dlDumpNoMemLeft();
    __apMemoryDebugAdd(dup, padded_size, file, line, AP_MEM_BT_ALLOCATOR);
    if (dup)
        strcpy((char *)dup, src);
    return (char *)dup;
}
