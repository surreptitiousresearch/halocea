#include "dsVECTOR.h"

#include "dsMsgAddrMapDUMMY.h"

// Debug global reallocator the ws build links in: dlRealloc(ptr, size, file, line).
extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<...msgADDR-map DUMMY*,8>::_internalRealloc @ 0x82556BB8
// Resize the backing store to `size` pointer elements. Element type is a bare pointer, so the
// storage is trivially relocatable: a single dlRealloc, no per-element copy/move.
template<>
void dsVECTOR<dsMsgAddrMapDUMMY *, 8>::_internalRealloc(int size)
{
    this->pData = (dsMsgAddrMapDUMMY **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
