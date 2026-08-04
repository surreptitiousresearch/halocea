#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct fioFILE_MEM; // boundary — fio in-memory file; element is a pointer.

// dsVECTOR<fioFILE_MEM*,8>::~dsVECTOR @ 0x82AC75E8
// Pointer elements are trivially destructible, so the destructor collapses to freeing the
// backing element storage.
template<>
dsVECTOR<fioFILE_MEM *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
