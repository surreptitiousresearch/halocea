#include "dsVECTOR.h"
#include "dsPARAM.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsPARAM,8>::ShrinkResize @ 0x823D8740
// Shrink the live count to `size`, destroying every dsPARAM from `size` up to the old count. The
// inlined ~dsPARAM dispatches Destroy on the parameter's type-erased value (`data.type`) when a
// value is present; the dsSTRID id needs no teardown.
template<>
void dsVECTOR<dsPARAM, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i) {
            dsPARAM *entry = &this->pData[i];
            if (entry->data.type)
                entry->data.type->Destroy(&entry->data.storage);
        }
        this->nElem = size;
    }
}
