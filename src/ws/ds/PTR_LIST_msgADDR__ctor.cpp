#include "PTR_LIST.h"
#include "../msg/msgADDR.h"

// ds::PTR_LIST<msgADDR,ds::LIST_NODE_ACCESS_POLICY<msgADDR>>::PTR_LIST @ 0x82555F98
template<>
ds::PTR_LIST<msgADDR, ds::LIST_NODE_ACCESS_POLICY<msgADDR> >::PTR_LIST()
{
    this->pHead = nullptr;
    this->pTail = nullptr;
    this->length = 0;
}
