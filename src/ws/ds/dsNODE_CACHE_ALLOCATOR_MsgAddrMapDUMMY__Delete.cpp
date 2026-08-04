#include "dsNODE_CACHE_ALLOCATOR.h"
#include "dsMsgAddrMapDUMMY.h"

// dsNODE_CACHE_ALLOCATOR<dsMsgAddrMapDUMMY>::Delete @ 0x8255A220 — alias for Release.
template<>
void dsNODE_CACHE_ALLOCATOR<dsMsgAddrMapDUMMY>::Delete(dsMsgAddrMapDUMMY *pNode)
{
    Release(pNode);
}
