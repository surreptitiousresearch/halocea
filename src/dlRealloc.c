/* dlRealloc @0x82506B68 — the engine's debug-tracked realloc. Reserves 6 extra bytes of allocator overhead,
 * updates the memory-debug ledger, and forwards to the apDLALLOC interface. A null `ptr` is a fresh Malloc; a
 * zero `size` on a live pointer is a Free returning null. Out-of-memory triggers dlDumpNoMemLeft.
 *
 * DEVIATION: this TU is C++, not C (see dlMalloc.c) — a FLAT export whose callees are all mangled.
 * DEVIATION: __apMemoryDebugRemove's second parameter is `bool` (?__apMemoryDebugRemove@@YAHPAX_N@Z);
 * spelling it `int` mangles to a symbol nothing defines. */

#include "headers/ws/ap/apDLALLOC_IFACE.h"
#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

/* ap memory-debug accounting (boundary, genuinely mangled). Both return int. */
extern int __apMemoryDebugRemove(void *ptr, bool flag);
extern int __apMemoryDebugAdd(void *ptr, unsigned int size, const char *file, unsigned int line, apMEM_BLOCK_TYPE kind);
extern void dlDumpNoMemLeft(void); /* ?dlDumpNoMemLeft@@YAXXZ */

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line)
{
    unsigned int actual_size;
    void *allocated;

    if ( ptr )
    {
        if ( !size )
        {
            __apMemoryDebugRemove(ptr, true);
            apDLALLOC_IFACE::Free(ptr);
            return nullptr;
        }
        __apMemoryDebugRemove(ptr, true);
        actual_size = size + 6;
        allocated = apDLALLOC_IFACE::Realloc(ptr, actual_size);
    }
    else
    {
        actual_size = size + 6;
        if ( size == (unsigned int)-6 )
            actual_size = 1;
        allocated = apDLALLOC_IFACE::Malloc(actual_size);
    }

    if ( !allocated )
        dlDumpNoMemLeft();
    /* disasm passes li r7,1 => AP_MEM_BT_ALLOCATOR (=1). Prior recon #define'd this
       to 0, which would have emitted the wrong block-type; corrected to the enum. */
    __apMemoryDebugAdd(allocated, actual_size, file, line, AP_MEM_BT_ALLOCATOR);
    return allocated;
}
