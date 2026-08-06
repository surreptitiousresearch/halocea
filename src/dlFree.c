/* dlFree @0x825066E0
 *
 * DEVIATION: this TU is C++, not C (see dlMalloc.c) — a FLAT export whose callees are all mangled.
 * DEVIATION: __apMemoryDebugRemove's second parameter is `bool`, not `int`: the binary symbol is
 * ?__apMemoryDebugRemove@@YAHPAX_N@Z and `_N` is bool. Spelling it `int` mangles to
 * ?__apMemoryDebugRemove@@YAHPAXH@Z, which nothing defines. */

#include "headers/ws/ap/apDLALLOC_IFACE.h"

int __apMemoryDebugRemove(void *ptr, bool is_allocator);

extern "C" void dlFree(void *ptr)
{
    if (ptr)
    {
        __apMemoryDebugRemove(ptr, true);
        apDLALLOC_IFACE::Free(ptr);
    }
}
