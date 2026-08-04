#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::Erase @ 0x82558398
// Erase returns a plain T* (trivial return), so unlike Insert/PushBack/Front/Back there is no
// hidden sret pointer and no ABI-role swap: r3=this, r4=where (by value), matching the
// corrected-in-PTR_LIST.h `T *Erase(ITERATOR it)` signature exactly. DB-verified via disasm
// (0x82558398-590): no register-role inversion present (single 2-param call, both roles confirmed
// by direct field reads matching `this`/`where` at face value).
template<>
msgADDR *ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::Erase(ITERATOR where)
{
    msgADDR *node = where.pNode;
    msgADDR *prevNode = ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Prev(node);
    msgADDR *nextNode = ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Next(node);

    if (prevNode)
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(prevNode, nextNode);
    else
    {
        this->pHead = nextNode;
        if (this->pHead)
            ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(this->pHead, nullptr);
    }

    if (nextNode)
        ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(nextNode, prevNode);
    else
    {
        this->pTail = prevNode;
        if (this->pTail)
            ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(this->pTail, nullptr);
    }

    ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(node, nullptr);
    ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(node, nullptr);
    --this->length;
    return node;
}
