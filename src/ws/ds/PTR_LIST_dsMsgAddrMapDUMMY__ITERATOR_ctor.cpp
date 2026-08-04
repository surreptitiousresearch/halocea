#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::ITERATOR::ITERATOR @ 0x82557BD8
template<>
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::ITERATOR::ITERATOR(dsMsgAddrMapDUMMY *node)
{
    this->pNode = node;
}
