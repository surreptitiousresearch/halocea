#include "dsVECTOR.h"

struct instCONSTRUCTOR_NODE; // boundary — ws-engine prop: instance-constructor graph node (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<instCONSTRUCTOR_NODE*,8>::~dsVECTOR @ 0x826DD308
// Pointer elements are not owned by the vector; just free the backing element storage.
template<>
dsVECTOR<instCONSTRUCTOR_NODE *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
