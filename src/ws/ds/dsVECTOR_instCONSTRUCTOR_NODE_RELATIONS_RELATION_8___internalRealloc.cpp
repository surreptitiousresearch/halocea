#include "dsVECTOR.h"
#include "../prop/instCONSTRUCTOR_NODE_RELATIONS.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION,8>::_internalRealloc @ 0x826DD100
// Resize the backing store to `size` elements via the debug reallocator (bare relocation; the
// container treats the 44-byte element as trivially relocatable here, matching Realloc).
template<>
void dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION, 8>::_internalRealloc(int size)
{
    this->pData = (instCONSTRUCTOR_NODE_RELATIONS::RELATION *)dlRealloc(
        this->pData, 44 * size, this->__cl.file, this->__cl.line);
}
