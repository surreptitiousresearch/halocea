#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::~dsVECTOR @ 0x82698320
// Pointer elements are trivially destructible, so the destructor collapses to freeing the
// backing element storage.
template<>
dsVECTOR<strmVID_BUFFER *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
