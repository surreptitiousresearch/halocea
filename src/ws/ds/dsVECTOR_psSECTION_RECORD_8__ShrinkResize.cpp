#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"
#include "ds_assert_boundary.h"

// dsVECTOR<psSECTION_RECORD,8>::ShrinkResize @ 0x825180C0
// Shrink the live count to `size`, destroying every record from `size` up to the old count. The
// inlined ~psSECTION_RECORD dispatches Destroy on the record's type-erased value (`value.type`)
// when a value is present; the interned key id and scalar fields need no teardown.
template<>
void dsVECTOR<psSECTION_RECORD, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i) {
            psSECTION_RECORD *entry = &this->pData[i];
            if (entry->value.type)
                entry->value.type->Destroy(&entry->value.storage);
        }
        this->nElem = size;
    }
}
