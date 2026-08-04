#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"
#include "../../hcex/hcex_light.h" // reuse the real HCEX_LIGHT layout

extern "C" long osLockedDecrement(volatile int *value); // boundary — atomic decrement

// dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8>::ShrinkResize @ 0x823D8E18
// Shrink the live count to `size`, releasing each pair above the new count. The int key is
// trivial; the HCEX_LIGHT value owns a weak reference to its ent binding: atomically drop the
// shared handle's refcount, delete the handle at zero, and null the slot.
template<>
void dsVECTOR<dsPAIR<int, HCEX_LIGHT>, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);

    if (size < this->nElem) {
        for (int index = size; index < this->nElem; ++index) {
            dsPAIR<int, HCEX_LIGHT> *pair = &this->pData[index];
            ds_WEAK_PTR_HANDLE *handle = pair->val.ent.pHandle;
            if (handle) {
                osLockedDecrement(&handle->refCount);
                if (handle->refCount <= 0)
                    operator delete(pair->val.ent.pHandle);
            }
            pair->val.ent.pHandle = nullptr;
        }
        this->nElem = size;
    }
}
