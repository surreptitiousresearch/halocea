#include "dsVECTOR.h"

#include "dsMsgAddrMapDUMMY.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<...msgADDR-map DUMMY*,8>::~dsVECTOR @ 0x82557790
// Destroy every live element (trivial for a raw pointer element, so the loop is empty) and
// release the backing storage.
template<>
dsVECTOR<dsMsgAddrMapDUMMY *, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        ; // trivial pointer destructor (empty)
    dlFree(this->pData);
}
