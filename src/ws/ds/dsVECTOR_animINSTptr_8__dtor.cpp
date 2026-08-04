#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

class animINST; // boundary — anim subsystem animation instance (element is a pointer)

// dsVECTOR<animINST*,8>::~dsVECTOR @ 0x823ED760
// Pointer elements are trivially destructible, so the destructor collapses to freeing the
// backing element storage.
template<>
dsVECTOR<animINST *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
