#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext @ 0x82556340
// Stores the address of pNext's EMBEDDED listNode sub-object (pNext+4), not pNext itself -- the
// convention Next() undoes with its -4 adjustment. DB-verified via decompile: `&pNext->PTR_LIST_NODE`.
template<>
void ds::LIST_NODE_ACCESS_POLICY<msgADDR>::SetNext(msgADDR *pNode, msgADDR *pNext)
{
    pNode->listNode.pNext = pNext ? &pNext->listNode : nullptr;
}
