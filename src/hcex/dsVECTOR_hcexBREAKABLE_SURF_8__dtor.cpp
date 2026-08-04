// dsVECTOR<hcexBREAKABLE_SURF,8>::~dsVECTOR() @ 0x823B1768
//
// Destroy the breakable-surface bridge vector: run scnINST_CREATE_DATA::~scnINST_CREATE_DATA on
// every live slot's embedded create-data (the only non-trivial member — bsp/idx/pInst are POD),
// then free the backing store. Reversed per-instantiation.

#include "../headers/hcex/hcex_surf_vector.h" // dsVECTOR<hcexBREAKABLE_SURF,8>, scnINST_CREATE_DATA

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

template<>
dsVECTOR<hcexBREAKABLE_SURF, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        this->pData[i].cd.~scnINST_CREATE_DATA();
    dlFree(this->pData);
}
