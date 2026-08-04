#include "dsVECTOR.h"
#include "../prop/instCONSTRUCTOR_NODE_RELATIONS.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION,8>::_internalFree @ 0x826DCFF0
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION, 8>::_internalFree(void *p)
{
    dlFree(p);
}
