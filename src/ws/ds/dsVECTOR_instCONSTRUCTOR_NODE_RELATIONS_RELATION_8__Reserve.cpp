#include "dsVECTOR.h"
#include "../prop/instCONSTRUCTOR_NODE_RELATIONS.h"

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION,8>::Reserve @ 0x826E1A00
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
