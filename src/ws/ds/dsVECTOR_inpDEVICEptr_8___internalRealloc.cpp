#include "dsVECTOR.h"

struct inpDEVICE; // boundary — ws-engine inp: physical input device (element is a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<inpDEVICE*,8>::_internalRealloc @ 0x8263F908
// Resize the backing store to hold exactly `size` elements via a single dlRealloc (pointer
// elements are trivially relocatable). Element size 4 bytes.
template<>
void dsVECTOR<inpDEVICE *, 8>::_internalRealloc(int size)
{
    this->pData = (inpDEVICE **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
