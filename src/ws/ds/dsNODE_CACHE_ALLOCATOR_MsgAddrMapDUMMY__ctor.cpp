#include "dsNODE_CACHE_ALLOCATOR.h"
#include "dsMsgAddrMapDUMMY.h"

// dsNODE_CACHE_ALLOCATOR<dsMsgAddrMapDUMMY>::dsNODE_CACHE_ALLOCATOR @ 0x8255BB58
// Forwards straight to the dsNODE_CACHE(apCL) base ctor.
template<>
dsNODE_CACHE_ALLOCATOR<dsMsgAddrMapDUMMY>::dsNODE_CACHE_ALLOCATOR(apCL cl)
    : dsNODE_CACHE<dsMsgAddrMapDUMMY>(cl)
{
}
