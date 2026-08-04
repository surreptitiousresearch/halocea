#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::Next @ 0x82556980
template<>
dsMsgAddrMapDUMMY *ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::Next(dsMsgAddrMapDUMMY *pNode)
{
    return pNode->pNext;
}
