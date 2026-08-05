#include <cstddef>
#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Prev @ 0x82556300
// Mirror of Next(): the raw stored pPrev points at the previous node's embedded listNode
// sub-object (prevAddr+4); subtract the 4-byte offset back off to recover the owning msgADDR*.
// DB-verified via disasm (0x82556300-30): loads *(pNode+4) [msgADDR::listNode.pPrev], `addi -4`.
template<>
msgADDR *ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Prev(msgADDR *pNode)
{
    ds::PTR_LIST_NODE *storedPrev = pNode->listNode.pPrev;
    if (!storedPrev)
        return nullptr;
    return (msgADDR *)((char *)storedPrev - offsetof(msgADDR, listNode));
}
