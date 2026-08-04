#include "dsVECTOR.h"

struct instCONSTRUCTOR_NODE; // boundary — ws-engine prop: instance-constructor graph node (element is a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<instCONSTRUCTOR_NODE*,8>::_internalRealloc @ 0x826DD0C0
// Resize the backing store to hold exactly `size` elements via a single dlRealloc (pointer
// elements are trivially relocatable). Element size 4 bytes.
template<>
void dsVECTOR<instCONSTRUCTOR_NODE *, 8>::_internalRealloc(int size)
{
    this->pData = (instCONSTRUCTOR_NODE **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
