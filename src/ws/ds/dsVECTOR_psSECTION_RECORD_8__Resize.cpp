#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"
#include "ds_assert_boundary.h"
#include <new>

// dsVECTOR<psSECTION_RECORD,8>::Resize @ 0x82519B40
// Resize the live count to `size`. Growing: reallocs if needed, then default-constructs each
// newly exposed slot (interns the empty-string key id, clears value.type) — mirrors CallNew's
// per-field init rather than a full placement-new, matching the disassembled inline sequence.
// Shrinking: destroys each dropped record's stored value (dispatch Destroy on value.type when
// present, same pattern as ShrinkResize/~dsVECTOR) before dropping the live count.
// DEVIATION: the decompile fabricates 12 phantom trailing int params (a3..a14) from a
// float-slot-skip ABI mis-shift; the real signature (matching the header's declared
// `Resize(int capacity)`) takes just the target size.
template<>
void dsVECTOR<psSECTION_RECORD, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size", size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int i = this->nElem; i < size; ++i) {
                psSECTION_RECORD *entry = &this->pData[i];
                new (&entry->keyStrId) dsSTRID();
                entry->value.type = nullptr;
            }
        }
        this->nElem = size;
    } else {
        for (int i = size; i < this->nElem; ++i) {
            psSECTION_RECORD *entry = &this->pData[i];
            if (entry->value.type)
                entry->value.type->Destroy(&entry->value.storage);
        }
        this->nElem = size;
    }
}
