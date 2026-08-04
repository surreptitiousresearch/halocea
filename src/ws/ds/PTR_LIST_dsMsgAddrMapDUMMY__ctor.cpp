#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::PTR_LIST @ 0x825568C0
template<>
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::PTR_LIST()
{
    this->pHead = nullptr;
    this->pTail = nullptr;
    this->length = 0;
}
