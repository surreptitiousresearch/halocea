#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

class dbgVAR; // boundary — dbg subsystem debug variable record (element is a pointer)

// dsVECTOR<dbgVAR*,8>::Erase @ 0x8255EBC0
// Remove `n` pointer elements starting at index `pos` by shifting the tail down over them.
// Elements are trivially relocatable (no per-element destructor runs).
template<>
void dsVECTOR<dbgVAR *, 8>::Erase(int pos, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || n < 0 || pos + n > this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && n >= 0 && pos + n <= nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            417,
            "pos", pos, "nElem", this->nElem, "n", n);
    if (n == 0)
        return;

    memmove(&this->pData[pos], &this->pData[pos + n],
            sizeof(dbgVAR *) * (this->nElem - pos - n));
    this->nElem -= n;
}
