#include "PTR_LIST.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::PTR_LIST<dsMsgAddrMapDUMMY,...>::C_ITERATOR::C_ITERATOR @ 0x82556430
template<>
ds::PTR_LIST<dsMsgAddrMapDUMMY, ds::LIST_NODE_ACCESS_POLICY<dsMsgAddrMapDUMMY> >::C_ITERATOR::C_ITERATOR(dsMsgAddrMapDUMMY *node)
{
    this->pNode = node;
}
