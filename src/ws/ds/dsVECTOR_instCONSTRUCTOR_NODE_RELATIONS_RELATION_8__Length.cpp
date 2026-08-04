#include "dsVECTOR.h"
#include "../prop/instCONSTRUCTOR_NODE_RELATIONS.h"

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION,8>::Length @ 0x826DCE10
// Number of live elements.
template<>
int dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION, 8>::Length() const
{
    return this->nElem;
}
