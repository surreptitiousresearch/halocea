#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev @ 0x82556380
// Mirror of SetNext(): stores the address of pPrev's embedded listNode sub-object.
template<>
void ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetPrev(msgADDR *pNode, msgADDR *pPrev)
{
    pNode->listNode.pPrev = pPrev ? &pPrev->listNode : nullptr;
}
