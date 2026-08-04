#include "dsNODE_CACHE.h"
#include "dsMsgAddrMapDUMMY.h"

// dsNODE_CACHE<dsMsgAddrMapDUMMY>::~dsNODE_CACHE @ 0x8255B6A0
// Release all owned node blocks (Term), then destroy nodeList.
template<>
dsNODE_CACHE<dsMsgAddrMapDUMMY>::~dsNODE_CACHE()
{
    Term();
}
