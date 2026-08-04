#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_OBJ.h" // reuse the real HCEX_OBJ layout

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free
// boundary — ~dsVECTOR<ds::WEAK_PTR<entENTITY>,8>: unlinks each weak ref and frees the storage.
extern void dsVECTOR_WEAK_PTR_entENTITY_8__dtor(dsVECTOR_followers *followers);

// dsVECTOR<HCEX_OBJ,8>::ShrinkResize @ 0x823EDCE0
// Shrink the live count to `size`, destroying every HCEX_OBJ from `size` up to the old count.
// The inlined ~HCEX_OBJ frees the followersOfs (m3dMATR) vector storage directly (trivially
// relocatable element) and runs the follower weak-pointer vector destructor.
template<>
void dsVECTOR<HCEX_OBJ, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i) {
            HCEX_OBJ *obj = &this->pData[i];
            dlFree(obj->followersOfs.pData);       // ~dsVECTOR<m3dMATR,8>: bare free
            dsVECTOR_WEAK_PTR_entENTITY_8__dtor(&obj->followers); // ~dsVECTOR<ds::WEAK_PTR<entENTITY>,8>
        }
        this->nElem = size;
    }
}
