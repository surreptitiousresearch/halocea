#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::ITERATOR::Get @ 0x82557DB8
template<>
dsMsgAddrMapDUMMY *ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::ITERATOR::Get() const
{
    return this->pNode;
}
