// dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8>::~dsVECTOR() @ 0x823DA968
//
// Destroy the light-binding table. Per element, the compiler inlined the dsPAIR<int,HCEX_LIGHT>
// destructor (which is itself the inlined HCEX_LIGHT destructor): drop the weak entity handle's
// reference, operator-delete it at a non-positive refcount, then null the handle. Finally free the
// backing store. Reversed per-instantiation.

#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/hcex_light.h"

extern "C" void dlFree(void *ptr);           // boundary — dlmalloc free (unmangled C export)
extern "C" int  osLockedDecrement(int *pVar); // boundary — src/ws/os/osLockedDecrement.cpp

template<>
dsVECTOR<hcex_light_pair, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
    {
        hcex_light_pair *pElem = &this->pData[i];
        ds_WEAK_PTR_HANDLE *handle = pElem->val.ent.pHandle;
        if (handle)
        {
            osLockedDecrement(const_cast<int *>(&handle->refCount));
            if (handle->refCount <= 0)
            {
                ds_WEAK_PTR_HANDLE *toFree = pElem->val.ent.pHandle;
                if (toFree)
                    operator delete(toFree);
            }
        }
        pElem->val.ent.pHandle = nullptr;
    }
    dlFree(this->pData);
}
