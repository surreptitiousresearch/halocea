#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct sslOBJECT; // boundary — ssl scripting object; element is a pointer.

// dsVECTOR<sslOBJECT*,8>::~dsVECTOR @ 0x82AD7E40
// Pointer elements are trivially destructible, so the destructor collapses to freeing the
// backing element storage.
template<>
dsVECTOR<sslOBJECT *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
