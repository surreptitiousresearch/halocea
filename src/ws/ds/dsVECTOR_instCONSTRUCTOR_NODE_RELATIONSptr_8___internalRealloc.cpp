#include "dsVECTOR.h"

struct instCONSTRUCTOR_NODE_RELATIONS; // boundary — ws-engine prop: instance-constructor node relation record (element is a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS*,8>::_internalRealloc @ 0x826DD140
// Resize the backing store to hold exactly `size` elements via a single dlRealloc (pointer
// elements are trivially relocatable). Element size 4 bytes.
template<>
void dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS *, 8>::_internalRealloc(int size)
{
    this->pData = (instCONSTRUCTOR_NODE_RELATIONS **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
