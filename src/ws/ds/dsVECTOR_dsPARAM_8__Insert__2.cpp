#include <string.h>
#include "dsVECTOR.h"
#include "dsPARAM.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsPARAM,8>::Insert(pos, val, n) @ 0x82D030D0
// Fill-splice `n` copies of the single element `val` at index `pos`. With spare capacity, splice
// directly. Otherwise `val` is staged into a local copy first (val may alias storage about to be
// reallocated) via the compiler-generated dsPARAM copy constructor (memberwise: id verbatim,
// data raw-copied) — NOT SetValue — then each new slot is built the same way as the range Insert,
// and the staged copy's dsDATA value is explicitly destroyed afterward.
template<>
void dsVECTOR<dsPARAM, 8>::Insert(int pos, const dsPARAM &val, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || n < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos, "nElem", this->nElem, "n", n);
    if (n == 0)
        return;

    int nElem = this->nElem;
    if (this->allocated >= nElem + n) {
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dsPARAM) * (nElem - pos));
        for (int i = 0; i < n; ++i) {
            dsPARAM *slot = &this->pData[pos + i];
            slot->id = dsSTRID();
            slot->data.type = nullptr;
            slot->id.id = val.id.id;
            slot->data.SetValue(&val.data);
        }
    } else {
        dsPARAM staged = val; // compiler-generated copy ctor: memberwise id + raw dsDATA copy

        int allocated = this->allocated;
        int needed = this->nElem + n;
        if (2 * allocated > needed)
            needed = 2 * allocated;
        if (needed > allocated)
            this->Realloc(needed);

        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(dsPARAM) * (this->nElem - pos));
        for (int i = 0; i < n; ++i) {
            dsPARAM *slot = &this->pData[pos + i];
            slot->id = dsSTRID();
            slot->data.type = nullptr;
            slot->id.id = staged.id.id;
            slot->data.SetValue(&staged.data);
        }
        if (staged.data.type) {
            staged.data.type->Destroy(&staged.data.storage);
            this->nElem += n;
            return;
        }
    }
    this->nElem += n;
}
