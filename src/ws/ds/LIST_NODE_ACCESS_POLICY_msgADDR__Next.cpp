#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Next @ 0x82556088
// msgADDR's intrusive link (msgADDR::listNode, a ds::PTR_LIST_NODE at offset 4) stores its
// pNext/pPrev as pointers to the NEXT/PREV node's EMBEDDED listNode sub-object (i.e. ownerAddr+4),
// not to the owning msgADDR itself -- see ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext, which
// stores `&pNext->listNode`. Next() must therefore subtract the 4-byte listNode offset back off
// the raw stored pointer to recover the owning msgADDR*. DB-verified via disasm (0x82556088-c0):
// loads *(pNode+8) [msgADDR::listNode.pNext], and if non-null does `addi r7,r8,-4` before returning.
template<>
msgADDR *ds::LIST_NODE_ACCESS_POLICY<msgADDR>::Next(msgADDR *pNode)
{
    ds::PTR_LIST_NODE *storedNext = pNode->listNode.pNext;
    if (!storedNext)
        return nullptr;
    return (msgADDR *)((char *)storedNext - 4);
}
