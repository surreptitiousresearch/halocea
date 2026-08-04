#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

// Debug global reallocator the ws build links in: dlRealloc(ptr, size, file, line).
extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<msgMSG,8>::_internalRealloc @ 0x82AC3DA0
// Resize the backing store to `size` elements. msgMSG (24 bytes) is a plain-old-data envelope
// holding no self-referencing pointers, so the storage is trivially relocatable: a single
// dlRealloc of 24*size bytes, no per-element copy/move (verified against the disassembly).
template<>
void dsVECTOR<msgMSG, 8>::_internalRealloc(int size)
{
    this->pData = (msgMSG *)dlRealloc(this->pData, 24 * size, this->__cl.file, this->__cl.line);
}
