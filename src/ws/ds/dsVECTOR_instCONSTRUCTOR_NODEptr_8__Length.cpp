#include "dsVECTOR.h"

struct instCONSTRUCTOR_NODE; // boundary — ws-engine prop: instance-constructor graph node (element is a pointer)

// dsVECTOR<instCONSTRUCTOR_NODE*,8>::Length @ 0x826DCE08
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<instCONSTRUCTOR_NODE *, 8>::Length() const
{
    return this->nElem;
}
