#include "dsNODE_CACHE_ALLOCATOR.h"
#include "dsMsgAddrMapDUMMY.h"

// dsNODE_CACHE_ALLOCATOR<dsMsgAddrMapDUMMY>::Alloc @ 0x8255C6A8 — alias for GetEmpty.
template<>
dsMsgAddrMapDUMMY *dsNODE_CACHE_ALLOCATOR<dsMsgAddrMapDUMMY>::Alloc()
{
    return GetEmpty();
}
