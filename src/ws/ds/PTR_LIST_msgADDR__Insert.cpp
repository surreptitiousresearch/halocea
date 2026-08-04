#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::Insert @ 0x82558178
// See the ABI-role CAVEAT in PTR_LIST_msgADDR__Front.cpp (applies identically here: r3=out
// iterator, r4=this, r5=val, r6=where -- val/where are unaffected by the swap, only this/result
// are). `where` is passed by value as a raw msgADDR* (ITERATOR is a single-pointer struct, so on
// this ABI it is passed directly in a register, not by reference, matching the corrected
// `T *Erase(ITERATOR it)`-style by-value convention documented in PTR_LIST.h).
//
// Splices `val` in immediately after `where.pNode`. When the list is empty, `val` becomes the sole
// element (where is ignored, matching the raw trace's `if (this->pHead)` branch). When `where.pNode`
// is the current tail, Next(where.pNode) is null, so `val` naturally becomes the new tail -- this is
// the "tail hook" case PTR_LIST.h's Insert doc comment refers to, and what PushBack relies on.
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::ITERATOR
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::Insert(msgADDR *val, ITERATOR where)
{
    ITERATOR result;

    if (this->pHead)
    {
        msgADDR *afterWhere = ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Next(where.pNode);
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(val, afterWhere);
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(val, where.pNode);
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(where.pNode, val);
        if (afterWhere)
            ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(afterWhere, val);
        else
            this->pTail = val;
    }
    else
    {
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(val, nullptr);
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(val, nullptr);
        this->pHead = val;
        this->pTail = val;
    }

    ++this->length;
    result.pNode = val;
    return result;
}
