// apDLALLOC_IFACE::Freealign @ 0x82A20750 — static, thunk to dlfree
// (aligned blocks free through the same dlfree path).
#include "apDLALLOC_IFACE.h"

void apDLALLOC_IFACE::Freealign(void *ptr)
{
    dlfree(ptr);
}
