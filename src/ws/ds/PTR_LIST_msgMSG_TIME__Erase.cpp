// ds::PTR_LIST<msgMSG_TIME,ds::LIST_NODE_ACCESS_POLICY<msgMSG_TIME>>::Erase @0x82AC4208 — unlink
// the node `where` refers to: splice its neighbours' next/prev pointers together (or update
// pHead/pTail when it was an endpoint), clear the iterator's stashed prev/next, decrement length.
#include <cstddef>
#include "../../headers/ws/ds/PTR_LIST.h"
#include "../../headers/ws/msg/msgMSG_TIME.h"

// The list threads ds::PTR_LIST_NODE hooks (msgMSG_TIME::listNode); recover the owning
// msgMSG_TIME* from a hook pointer by undoing the listNode offset.
static inline msgMSG_TIME *ownerOf(ds::PTR_LIST_NODE *hook)
{
    return hook ? (msgMSG_TIME *)((char *)hook - offsetof(msgMSG_TIME, listNode)) : nullptr;
}

// DB mangle QAAPAVmsgMSG_TIME: returns the unlinked node T*, not an ITERATOR
template<>
msgMSG_TIME *ds::PTR_LIST<msgMSG_TIME>::Erase(ITERATOR where)
{
    msgMSG_TIME *node = where.pNode;
    // prev/next are hook pointers (PTR_LIST_NODE*), not msgMSG_TIME*
    ds::PTR_LIST_NODE *prev = node->listNode.pPrev;
    ds::PTR_LIST_NODE *next = node->listNode.pNext;

    if (prev)
        prev->pNext = next;
    else {
        pHead = ownerOf(next);
        if (next)
            next->pPrev = nullptr;
    }

    if (next)
        next->pPrev = prev;
    else {
        pTail = ownerOf(prev);
        if (prev)
            prev->pNext = nullptr;
    }

    node->listNode.pPrev = nullptr;
    node->listNode.pNext = nullptr;
    --length;
    return node;
}
