#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<camCAMERA*,8>::~dsVECTOR @ 0x82703CB0
// Pointer elements are not owned by the vector; just free the backing element storage.
template<>
dsVECTOR<camCAMERA *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
