#include <string.h>
#include "dsVECTOR.h"
#include "dsPARAM.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsPARAM,8>::Insert(pos, begin, count) @ 0x827EE1D0
// Range-splice `count` parameters from `begin` at index `pos`: grow (doubling policy), memmove the
// tail up, then copy-construct each new slot's id (dsSTRID default ctor, then adopt the interned
// pointer directly) and copy its dsDATA value via SetValue.
template<>
void dsVECTOR<dsPARAM, 8>::Insert(int pos, const dsPARAM *begin, int count)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || count < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && num >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            382,
            "pos", pos, "nElem", this->nElem, "num", count);
    if (count == 0)
        return;

    int needed = this->nElem + count;
    dsPARAM *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(dsPARAM) * (this->nElem - pos));
    for (int i = 0; i < count; ++i) {
        dsPARAM *slot = &this->pData[pos + i];
        slot->id = dsSTRID();          // dsSTRID::dsSTRID() default ctor
        slot->data.type = nullptr;
        slot->id.id = begin[i].id.id;
        slot->data.SetValue(&begin[i].data);
    }
    this->nElem += count;
}
