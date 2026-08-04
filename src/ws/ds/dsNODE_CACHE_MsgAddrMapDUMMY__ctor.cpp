#include "dsNODE_CACHE.h"
#include "dsMsgAddrMapDUMMY.h"

// dsNODE_CACHE<dsMsgAddrMapDUMMY>::dsNODE_CACHE @ 0x82557BF8
// Zero-init nNode/nodeList/freeNodeList and copy the caller's call-site cookie into __cl.
// DEVIATION/CAVEAT: the raw decompile ("local variable allocation has failed") shows the trailing
// apCL store corrupted (`__cl.file = this`, `__cl.line = &cl`, both nonsensical for an apCL) —
// reconstructed as the obviously-intended `__cl = *cl` copy, matching every sibling ctor in this
// corpus that takes an apCL-by-pointer-in-the-decompile/by-value-per-mangle parameter.
template<>
dsNODE_CACHE<dsMsgAddrMapDUMMY>::dsNODE_CACHE(apCL cl)
{
    nNode = 0;
    nodeList.pData = nullptr;
    nodeList.nElem = 0;
    nodeList.allocated = 0;
    nodeList.__cl = cl;
    freeNodeList.pHead = nullptr;
    freeNodeList.pTail = nullptr;
    freeNodeList.length = 0;
    __cl = cl;
}
