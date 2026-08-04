#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

class msgADDR; // boundary — msg-subsystem message address (element is a pointer)

// dsVECTOR<msgADDR*,8>::Erase @ 0x82559968
// Remove `n` pointer elements starting at index `pos` by shifting the tail down over them and
// dropping the live count. Elements are trivially relocatable (no per-element destructor runs).
template<>
void dsVECTOR<msgADDR *, 8>::Erase(int pos, int n)
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
            sizeof(msgADDR *) * (this->nElem - pos - n));
    this->nElem -= n;
}
