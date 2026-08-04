#include <string.h>
#include "dsVECTOR.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsDATA,8>::Erase @ 0x8261C370
// Remove `n` elements starting at index `pos`: destroy each removed element via its own stored
// type's Destroy hook (disasm: the virtual call target is &pData[i].storage, which is &pData[i]
// itself since `storage` is dsDATA's offset-0 field — matches the dsDATA::Destroy convention used
// throughout this vector's other methods), then memmove the tail down over the gap and drop the
// live count. Empty (null-type) slots are skipped.
template<>
void dsVECTOR<dsDATA, 8>::Erase(int pos, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || n < 0 || pos + n > this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && n >= 0 && pos + n <= nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            417,
            "pos", pos, "nElem", this->nElem, "n", n);
    if (n == 0)
        return;

    for (int i = pos; i < pos + n; ++i) {
        const dsDATA_TYPE *type = this->pData[i].type;
        if (type)
            type->Destroy(&this->pData[i].storage);
    }
    memmove(&this->pData[pos], &this->pData[pos + n], sizeof(dsDATA) * (this->nElem - pos - n));
    this->nElem -= n;
}
