/* dlFreeAligned @0x82506720
 *
 * DEVIATION: this TU is C++, not C (see dlMalloc.c) — a FLAT export whose callees are all mangled.
 * DEVIATION: __apMemoryDebugRemove's second parameter is `bool` (?__apMemoryDebugRemove@@YAHPAX_N@Z). */

#include "headers/ws/ap/apDLALLOC_IFACE.h"

int __apMemoryDebugRemove(void *ptr, bool is_allocator);

extern "C" void dlFreeAligned(void *ptr)
{
    if (ptr)
    {
        __apMemoryDebugRemove(ptr, true);
        apDLALLOC_IFACE::Freealign(ptr);
    }
}
