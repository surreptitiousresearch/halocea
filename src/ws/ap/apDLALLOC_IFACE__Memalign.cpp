// apDLALLOC_IFACE::Memalign @ 0x82A20720 — static, thunk to dlmemalign.
// Note the argument swap: dlmemalign takes (align, size).
#include "apDLALLOC_IFACE.h"

void *apDLALLOC_IFACE::Memalign(unsigned int size, unsigned int align)
{
    return dlmemalign(align, size);
}
