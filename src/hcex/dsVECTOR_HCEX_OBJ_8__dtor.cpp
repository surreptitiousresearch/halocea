// dsVECTOR<HCEX_OBJ,8>::~dsVECTOR() @ 0x823EE0C0
//
// Destroy the hcexObjects sorted vector. For each live element the HCEX_OBJ destructor is inlined:
// free the followersOfs (dsVECTOR<m3dMATR,8>) backing store, then run the followers
// (dsVECTOR<ds::WEAK_PTR<entENTITY>,8>) destructor — WEAK_PTR is trivially destructible, so that
// reduces to freeing the followers backing store. Then free the vector's own backing store.
// Reversed per-instantiation.

#include "../headers/hcex/HCEX_OBJ.h"
#include "../headers/ws/ds/dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

template<>
dsVECTOR<HCEX_OBJ, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
    {
        HCEX_OBJ *obj = &this->pData[i];
        dlFree(obj->followersOfs.pData);          // inlined followersOfs dsVECTOR dtor
        dlFree(obj->followers.pData);             // inlined followers dsVECTOR dtor (trivial element)
    }
    dlFree(this->pData);
}
