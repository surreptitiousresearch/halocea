#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::PushBack @ 0x82558C58
// Forwards into Insert() with `where` wrapping the current tail (same tail-hook convention as the
// msgADDR instantiation's PushBack; DB-verified via decompile: `Insert(this,result,val,
// (ITERATOR*)result[1].pNode)` i.e. where = this->pTail).
template<>
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::ITERATOR
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::PushBack(dsMsgAddrMapDUMMY *val)
{
    return this->Insert(val, ITERATOR(this->pTail));
}
