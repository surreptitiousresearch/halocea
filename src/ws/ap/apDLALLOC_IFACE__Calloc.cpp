// apDLALLOC_IFACE::Calloc @0x82A20730 — thin forward to the dlmalloc-family zero-filling
// allocator (n=1 element of `size` bytes).
#include "../../headers/ws/ap/apDLALLOC_IFACE.h"

extern "C" void *dlcalloc(unsigned int size, unsigned int n);

void *apDLALLOC_IFACE::Calloc(unsigned int size)
{
    return dlcalloc(size, 1);
}
