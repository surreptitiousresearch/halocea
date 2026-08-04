#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::ITERATOR::operator* @ 0x82557D78
template<>
dsMsgAddrMapDUMMY &ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::ITERATOR::operator*() const
{
    return *this->pNode;
}
