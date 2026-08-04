#include "dsVECTOR.h"
#include "../ssl/sslSTACK_LOCAL_VAR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslSTACK::LOCAL_VAR,8>::ShrinkResize @ 0x82AF13F0
// Shrink the live count to `size`, destroying every LOCAL_VAR from `size` up to the old count.
// Each carries a dsDATA value whose runtime type descriptor's Destroy virtual tears down the
// stored word (skipping empty slots whose type is null).
template<>
void dsVECTOR<sslSTACK::LOCAL_VAR, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            sslSTACK::LOCAL_VAR *elem = &this->pData[index];
            if (elem->val.type)
                // Decompiler rendered this as a 1-arg indirect call; Destroy (vtable slot 0xC)
                // actually takes the storage pointer in r4 — verified against disasm at 0x82AF1488.
                elem->val.type->Destroy(&elem->val.storage);
        }
        this->nElem = size;
    }
}
