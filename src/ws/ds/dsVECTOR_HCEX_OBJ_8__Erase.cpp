#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_OBJ.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc free

// boundary — the inlined dsVECTOR<ds::WEAK_PTR<entENTITY>,8> destructor: it drops each stored weak
// reference (unlinking it from its entity) and releases the backing storage. Represented as a free
// function here because HCEX_OBJ.h models the member as a plain C struct.
void DestroyFollowersVector(dsVECTOR_followers *followers); // boundary (non-static: defined elsewhere)

// dsVECTOR<HCEX_OBJ,8>::Erase @ 0x823EE120
// Remove `n` 56-byte HCEX_OBJ elements starting at `pos`. Each element owns two vectors: the
// followersOfs (dsVECTOR<m3dMATR,8>, trivially destructible so only its storage is freed) and the
// followers (dsVECTOR<ds::WEAK_PTR<entENTITY>,8>, whose destructor unlinks each weak ref). The
// erased range is destroyed first, then the tail is shifted down and the live count drops.
template<>
void dsVECTOR<HCEX_OBJ, 8>::Erase(int pos, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || n < 0 || pos + n > this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && n >= 0 && pos + n <= nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            417,
            "pos", pos,
            "nElem", this->nElem,
            "n", n);

    if (n) {
        for (int eraseIndex = pos; eraseIndex < pos + n; ++eraseIndex) {
            HCEX_OBJ *element = &this->pData[eraseIndex];
            dlFree(element->followersOfs.pData);
            DestroyFollowersVector(&element->followers);
        }
        memmove(&this->pData[pos], &this->pData[pos + n], 56 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
