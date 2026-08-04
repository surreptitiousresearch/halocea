#include "msgSYSTEM.h"

// msgSYSTEM::ITERATOR::ITERATOR(int, ds::PTR_LIST<msgADDR,...>::ITERATOR) @ 0x82555B60
// Plain void-returning ctor (no sret involved), so no ABI-role swap: this=r3, ind1=r4, it=r5
// (verified via disasm 0x82555b60-88 -- straightforward two-field store, no register inversion).
msgSYSTEM::ITERATOR::ITERATOR(int ind1, ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR it)
{
    this->ind1 = ind1;
    this->it.pNode = it.pNode;
}
