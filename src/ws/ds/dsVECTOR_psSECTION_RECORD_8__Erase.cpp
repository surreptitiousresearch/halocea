#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ps/psSECTION_RECORD.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsVECTOR<psSECTION_RECORD,8>::Erase @ 0x82518510
// Remove `n` elements starting at `pos`. Each psSECTION_RECORD's dsDATA `value` may hold a
// constructed value; destroy it through the runtime type descriptor's Destroy vtable slot before
// dropping the record. Then shift the tail down over the gap (20-byte elements) and drop the count.
template<>
void dsVECTOR<psSECTION_RECORD, 8>::Erase(int pos, int n)
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
        for (int eraseIndex = pos; eraseIndex < pos + n; ++eraseIndex) {
            psSECTION_RECORD *record = &this->pData[eraseIndex];
            if (record->value.type)
                // decompiler dropped the storage arg; Destroy operates on the value's storage word.
                record->value.type->Destroy(&record->value.storage);
        }
        memmove(&this->pData[pos], &this->pData[pos + n], 20 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
