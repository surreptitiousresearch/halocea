#include <string.h>
#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"
#include "ds_assert_boundary.h"

// dsVECTOR<mtlMTL,8>::Erase @ 0x82C2AFE8
// Remove `n` elements starting at index `pos`: destroy each removed element, then shift the tail
// down over the gap with memmove.
// NOTE: the decompiler emitted extra phantom int params (a4..a16) from the STRONG_ASSERT2 varargs
// helper folded into the caller's frame — only `pos` and `n` are real parameters.
template<>
void dsVECTOR<mtlMTL, 8>::Erase(int pos, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || n < 0 || pos + n > this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && n >= 0 && pos + n <= nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            417,
            "pos", pos,
            "nElem", this->nElem,
            "n", n);

    if (n != 0) {
        for (int index = pos; index < pos + n; ++index)
            this->pData[index].~mtlMTL();
        memmove(&this->pData[pos], &this->pData[pos + n], 60 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
