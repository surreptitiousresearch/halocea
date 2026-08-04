#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::~dsVECTOR @ 0x82698328
// Pointer elements are trivially destructible, so the destructor collapses to freeing the
// backing element storage.
template<>
dsVECTOR<strmVID_MEM_BLOCK *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
