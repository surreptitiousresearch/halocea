#include <string.h>
#include "dsVECTOR.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsSTRID,8>::Erase @ 0x8252D680
// Remove `n` elements starting at index `pos` by shifting the tail down over them (memmove) and
// dropping the live count. dsSTRID is trivially relocatable — no per-element destructor runs.
template<>
void dsVECTOR<dsSTRID, 8>::Erase(int pos, int n)
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
        memmove(&this->pData[pos], &this->pData[pos + n], sizeof(dsSTRID) * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
