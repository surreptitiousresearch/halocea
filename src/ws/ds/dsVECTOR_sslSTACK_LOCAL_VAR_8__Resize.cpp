#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ssl/sslSTACK_LOCAL_VAR.h"

// dsVECTOR<sslSTACK::LOCAL_VAR,8>::Resize @ 0x82AF2690
// Resize the live count to `size`. Growing past capacity reallocates, then default-constructs the
// newly exposed locals (empty value: type descriptor nulled). Shrinking destroys each dropped
// local by dispatching its dsDATA value's runtime-type Destroy virtual (skipping empty slots).
template<>
void dsVECTOR<sslSTACK::LOCAL_VAR, 8>::Resize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            211,
            "size",
            size);

    int nElem = this->nElem;
    if (size >= nElem) {
        if (size > nElem) {
            if (size > this->allocated)
                this->Realloc(size);
            for (int index = this->nElem; index < size; ++index)
                this->pData[index].val.type = 0; // default-construct: empty type-erased value
        }
        this->nElem = size;
    } else {
        for (int index = size; index < this->nElem; ++index) {
            sslSTACK::LOCAL_VAR *local = &this->pData[index];
            if (local->val.type)
                // Decompiler rendered this as a 1-arg indirect call; Destroy (vtable slot 0xC)
                // actually takes the storage pointer in r4 — verified against disasm at 0x82AF2728.
                local->val.type->Destroy(&local->val.storage);
        }
        this->nElem = size;
    }
}
