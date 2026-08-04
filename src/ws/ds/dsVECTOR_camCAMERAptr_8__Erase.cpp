#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

// dsVECTOR<camCAMERA*,8>::Erase @ 0x82704070
// Remove `n` pointer elements starting at index `pos` by shifting the tail down over them and
// dropping the live count. Elements are trivially relocatable (no per-element destructor runs).
template<>
void dsVECTOR<camCAMERA *, 8>::Erase(int pos, int n)
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
            sizeof(camCAMERA *) * (this->nElem - pos - n));
    this->nElem -= n;
}
