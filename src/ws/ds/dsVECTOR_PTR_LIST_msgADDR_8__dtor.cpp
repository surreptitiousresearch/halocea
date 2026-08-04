#include "dsVECTOR.h"
#include "PTR_LIST.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::~dsVECTOR @ 0x82557958
// Destroy every live element (trivial — each element is a PTR_LIST head record with no owned
// storage, so the loop is empty) and release the backing storage.
template<>
dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        ; // trivial element destructor (empty)
    dlFree(this->pData);
}
