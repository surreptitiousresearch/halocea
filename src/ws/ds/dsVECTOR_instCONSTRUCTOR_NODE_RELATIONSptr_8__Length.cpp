#include "dsVECTOR.h"

struct instCONSTRUCTOR_NODE_RELATIONS; // boundary — ws-engine prop: instance-constructor node relation record (element is a pointer)

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS*,8>::Length @ 0x826DCE18
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS *, 8>::Length() const
{
    return this->nElem;
}
