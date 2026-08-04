#include "dsNODE_CACHE.h"
#include "dsObjOBJVecMapDUMMY.h"

// dsNODE_CACHE<dsObjOBJVecMapDUMMY>::dsNODE_CACHE(apCL) @ 0x826844C0
// Default-construct an empty node pool tagged with the caller's allocation call-site cookie.
// DEVIATION: decompile flagged "local variable allocation has failed" (raw DWORD-array writes
// through an untyped pointer); re-derived against the DB-verified nNode@0/nodeList@4/
// freeNodeList@24/__cl@36 layout.
template<>
dsNODE_CACHE<dsObjOBJVecMapDUMMY>::dsNODE_CACHE(apCL cl)
{
    this->nNode = 0;
    this->nodeList.pData = nullptr;
    this->nodeList.nElem = 0;
    this->nodeList.allocated = 0;
    this->nodeList.__cl = cl;
    this->freeNodeList.pHead = nullptr;
    this->freeNodeList.pTail = nullptr;
    this->freeNodeList.length = 0;
    this->__cl = cl;
}
