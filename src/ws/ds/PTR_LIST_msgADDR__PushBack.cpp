#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::PushBack @ 0x82558938
// Forwards straight into Insert(), passing an ITERATOR wrapping the current tail as the "where"
// position (Insert's documented "tail hook" case: DB-verified disasm shows PushBack loading
// `where = this->pTail` and reusing its own sret/this registers unchanged as Insert's args).
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::PushBack(msgADDR *val)
{
    return this->Insert(val, ITERATOR(this->pTail));
}
