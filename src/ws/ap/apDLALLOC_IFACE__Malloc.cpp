// apDLALLOC_IFACE::Malloc @ 0x82A206B0 — static.
// When the global default alignment is 16, route through dlmemalign(16, size);
// otherwise a plain dlmalloc. def_align is read atomically.
#include "apDLALLOC_IFACE.h"

void *apDLALLOC_IFACE::Malloc(unsigned int size)
{
    if (osLockedCompareAndSwap(&def_align, 0, 0) == 16)
        return dlmemalign(0x10u, size);
    return dlmalloc(size);
}
