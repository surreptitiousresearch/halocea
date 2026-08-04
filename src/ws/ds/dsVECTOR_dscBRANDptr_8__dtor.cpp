#include "dsVECTOR.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dscBRAND*,8>::~dsVECTOR @ 0x82663430
// Pointer elements are not owned by the vector; just free the backing element storage.
template<>
dsVECTOR<dscBRAND *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
