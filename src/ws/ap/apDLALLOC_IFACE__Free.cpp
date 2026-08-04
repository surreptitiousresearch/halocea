// apDLALLOC_IFACE::Free @ 0x82A20718 — static, thunk to dlfree.
#include "apDLALLOC_IFACE.h"

void apDLALLOC_IFACE::Free(void *ptr)
{
    dlfree(ptr);
}
