#include <string.h>
#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslCLASS_MAP_ELEM,8>::Erase @ 0x825CC9E0
// Remove `n` elements starting at index `pos`: destroy each removed element's cls ref, shift the
// tail down over them (memmove), and drop the live count.
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::Erase(int pos, int n)
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
        for (int i = pos; i < pos + n; ++i)
            this->pData[i].cls.~sslCLASS_REF();
        memmove(&this->pData[pos], &this->pData[pos + n], sizeof(sslCLASS_MAP_ELEM) * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
