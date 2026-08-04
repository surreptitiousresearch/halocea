#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::C_ITERATOR::operator-> @ 0x82556BA8
template<>
const dsMsgAddrMapDUMMY *ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::C_ITERATOR::operator->() const
{
    return this->pNode;
}
