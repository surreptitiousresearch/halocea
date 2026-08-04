#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../con_/CON_MSG.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc allocator free

// dsVECTOR<con::CON_MSG,8>::Erase @ 0x8309AFC8
// Remove `n` elements starting at `pos`. Each con::CON_MSG owns a heap payload buffer (its `data`
// vector's pData); free that buffer for every erased element, then shift the tail down over the gap
// (24-byte elements) and drop the live count.
template<>
void dsVECTOR<con::CON_MSG, 8>::Erase(int pos, int n)
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
        for (int eraseIndex = pos; eraseIndex < pos + n; ++eraseIndex)
            dlFree(this->pData[eraseIndex].data.pData);
        memmove(&this->pData[pos], &this->pData[pos + n], 24 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
