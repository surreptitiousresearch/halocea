// apDLALLOC_IFACE::ReallocAllign @ 0x82A20740 — static, thunk to dlrealloc_align.
// Note the argument order: dlrealloc_align takes (ptr, align, size).
#include "apDLALLOC_IFACE.h"

void *apDLALLOC_IFACE::ReallocAllign(void *ptr, unsigned int size, unsigned int align)
{
    return dlrealloc_align(ptr, align, size);
}
