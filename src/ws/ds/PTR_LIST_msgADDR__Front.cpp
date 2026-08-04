#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::Front @ 0x825580D0
//
// CAVEAT (applies to every ITERATOR/C_ITERATOR-by-value-returning PTR_LIST<msgADDR> method:
// Front/Back/FrontConst/PushBack/PushFront/Insert): this platform's ABI for a non-trivial-by-value
// return places the hidden hidden hidden sret-out pointer in r3 and `this` in r4 (this-pointer is
// bumped to the second slot rather than occupying r3). IDA's decompiler mis-names the two params in
// DECLARATION order ("this" for the 1st/r3, "result" for the 2nd/r4) rather than by role, which
// reads backwards (e.g. Front's raw trace is literally `this->pHead = result->pNode`, which would
// corrupt the list if taken at face value). Verified definitively via disasm (0x825580D0-f4): reads
// *(r4+0), writes *(r3+0), returns r3 -- the only reading consistent with a real, non-corrupting
// Front() is r3=out-iterator, r4=this. All these bodies below are written using the CORRECTED roles.
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::Front()
{
    ITERATOR result;
    result.pNode = this->pHead;
    return result;
}
