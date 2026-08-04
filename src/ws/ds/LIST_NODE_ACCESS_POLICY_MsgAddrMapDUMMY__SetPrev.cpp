#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetPrev @ 0x825569A8
template<>
void ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetPrev(dsMsgAddrMapDUMMY *pNode, dsMsgAddrMapDUMMY *pPrev)
{
    pNode->pPrev = pPrev;
}
