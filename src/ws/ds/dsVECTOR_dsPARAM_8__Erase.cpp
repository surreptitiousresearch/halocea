#include <string.h>
#include "dsVECTOR.h"
#include "dsPARAM.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsPARAM,8>::Erase @ 0x8262DE20
// Remove `n` elements starting at `pos`: destroy each (dsPARAM::~dsPARAM, which tears down its
// dsDATA value), then shift the tail down over the hole (memmove) and drop the live count.
template<>
void dsVECTOR<dsPARAM, 8>::Erase(int pos, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || n < 0 || pos + n > this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && n >= 0 && pos + n <= nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            417,
            "pos", pos, "nElem", this->nElem, "n", n);

    if (n) {
        for (int index = pos; index < pos + n; ++index)
            this->pData[index].~dsPARAM();
        memmove(&this->pData[pos], &this->pData[pos + n], sizeof(dsPARAM) * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
