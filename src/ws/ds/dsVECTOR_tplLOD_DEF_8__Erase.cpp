#include <string.h>
#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"
#include "ds_assert_boundary.h"

// dsVECTOR<tplLOD_DEF,8>::Erase @ 0x82B14228
// Remove `n` elements starting at index `pos` by shifting the tail down over them (memmove) and
// dropping the live count. tplLOD_DEF is trivially relocatable — no per-element destructor runs.
template<>
void dsVECTOR<tplLOD_DEF, 8>::Erase(int pos, int n)
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
        memmove(&this->pData[pos], &this->pData[pos + n], sizeof(tplLOD_DEF) * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
