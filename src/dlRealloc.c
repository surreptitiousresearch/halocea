/* dlRealloc @0x82506B68 — the engine's debug-tracked realloc. Reserves 6 extra bytes of allocator overhead,
 * updates the memory-debug ledger, and forwards to the apDLALLOC interface. A null `ptr` is a fresh Malloc; a
 * zero `size` on a live pointer is a Free returning null. Out-of-memory triggers dlDumpNoMemLeft. */

#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

/* ap memory-debug accounting. __apMemoryDebugRemove second arg is a bool (mangled _N),
 * passed as 1 (r4=1) here; kept as int per corpus convention. Both return int. */
extern int __apMemoryDebugRemove(void *ptr, int flag);
extern int __apMemoryDebugAdd(void *ptr, unsigned int size, const char *file, unsigned int line, apMEM_BLOCK_TYPE kind);
extern void dlDumpNoMemLeft(void);

/* apDLALLOC_IFACE namespaced allocator entry points (ws/ap platform layer) */
extern void  apDLALLOC_IFACE_Free(void *ptr);
extern void *apDLALLOC_IFACE_Realloc(void *ptr, unsigned int size);
extern void *apDLALLOC_IFACE_Malloc(unsigned int size);

void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line)
{
    unsigned int actual_size;
    void *allocated;

    if ( ptr )
    {
        if ( !size )
        {
            __apMemoryDebugRemove(ptr, 1);
            apDLALLOC_IFACE_Free(ptr);
            return nullptr;
        }
        __apMemoryDebugRemove(ptr, 1);
        actual_size = size + 6;
        allocated = apDLALLOC_IFACE_Realloc(ptr, actual_size);
    }
    else
    {
        actual_size = size + 6;
        if ( size == (unsigned int)-6 )
            actual_size = 1;
        allocated = apDLALLOC_IFACE_Malloc(actual_size);
    }

    if ( !allocated )
        dlDumpNoMemLeft();
    /* disasm passes li r7,1 => AP_MEM_BT_ALLOCATOR (=1). Prior recon #define'd this
       to 0, which would have emitted the wrong block-type; corrected to the enum. */
    __apMemoryDebugAdd(allocated, actual_size, file, line, AP_MEM_BT_ALLOCATOR);
    return allocated;
}
