#include "dsVECTOR.h"
#include "PTR_LIST.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

// Debug global reallocator the ws build links in: dlRealloc(ptr, size, file, line).
extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::_internalRealloc @ 0x82556D60
// Resize the backing store to `size` elements. Each element is a ds::PTR_LIST head record
// (pHead/pTail/length = 12 bytes) which holds no self-referencing pointers, so the storage is
// trivially relocatable: a single dlRealloc of 12*size bytes, no per-element copy/move.
template<>
void dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::_internalRealloc(int size)
{
    this->pData = (ds::PTR_LIST<msgADDR> *)dlRealloc(this->pData, 12 * size, this->__cl.file, this->__cl.line);
}
