#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::IsEmpty @ 0x82556B88
template<>
bool ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::IsEmpty() const
{
    return this->pHead == nullptr;
}
