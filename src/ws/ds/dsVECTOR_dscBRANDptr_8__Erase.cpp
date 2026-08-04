#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

// dsVECTOR<dscBRAND*,8>::Erase @ 0x827103C8
// Remove `n` pointer elements starting at index `pos` by shifting the tail down over them and
// dropping the live count. Elements are trivially relocatable (no per-element destructor runs).
template<>
void dsVECTOR<dscBRAND *, 8>::Erase(int pos, int n)
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
            sizeof(dscBRAND *) * (this->nElem - pos - n));
    this->nElem -= n;
}
