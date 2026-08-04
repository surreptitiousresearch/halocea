#include "dsVECTOR.h"
#include "PTR_LIST.h"
#include "ds_assert_boundary.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::Erase @ 0x82559760
// Remove `n` 12-byte PTR_LIST head-record elements starting at `pos`. Each element holds no owned
// storage (pHead/pTail/length observer fields into the intrusive msgADDR list), so the per-element
// destructor loop is empty; the tail is shifted down over the erased range and the live count
// drops. DEVIATION: the decompiler's assert-arg dump shows the third STRONG_ASSERT2_HELPER::asserd
// argument as an unnamed dangling pointer (`v16`, register-allocation-failure artifact); corrected
// to `n` per the assert's own condition string and the matching pattern in the sibling
// dsVECTOR<HCEX_OBJ,8>::Erase reconstruction.
template<>
void dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::Erase(int pos, int n)
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
            ; // trivial element destructor (empty)
        memmove(&this->pData[pos], &this->pData[pos + n], 12 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
