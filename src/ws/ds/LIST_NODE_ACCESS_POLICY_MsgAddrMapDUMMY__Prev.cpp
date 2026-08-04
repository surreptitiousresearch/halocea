#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::Prev @ 0x82556CC8
template<>
dsMsgAddrMapDUMMY *ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY>::Prev(dsMsgAddrMapDUMMY *pNode)
{
    return pNode->pPrev;
}
