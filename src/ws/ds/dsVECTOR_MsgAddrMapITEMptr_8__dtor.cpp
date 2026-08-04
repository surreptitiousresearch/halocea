#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<...msgADDR-map ITEM*,8>::~dsVECTOR @ 0x82557718
// Destroy every live element (trivial for a raw pointer element, so the loop is empty) and
// release the backing storage.
template<>
dsVECTOR<dsMsgAddrMapITEM *, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        ; // trivial pointer destructor (empty)
    dlFree(this->pData);
}
