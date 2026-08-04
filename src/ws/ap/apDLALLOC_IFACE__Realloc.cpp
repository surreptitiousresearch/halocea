// apDLALLOC_IFACE::Realloc @ 0x82A20738 — static, thunk to dlrealloc.
#include "apDLALLOC_IFACE.h"

void *apDLALLOC_IFACE::Realloc(void *ptr, unsigned int size)
{
    return dlrealloc(ptr, size);
}
