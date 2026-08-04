#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetNext @ 0x82556990
template<>
void ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::SetNext(dsMsgAddrMapDUMMY *pNode, dsMsgAddrMapDUMMY *pNext)
{
    pNode->pNext = pNext;
}
