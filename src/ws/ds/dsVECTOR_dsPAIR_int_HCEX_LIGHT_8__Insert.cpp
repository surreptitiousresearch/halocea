#include <string.h>
#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/hcex_light.h"

// os interlocked ref-count ops on the weak-pointer handle (os subsystem — not reversed here).
extern "C" long osLockedIncrement(volatile int *value);
extern "C" long osLockedDecrement(volatile int *value);

// dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8>::Insert @ 0x823E2CC0
// Fill overload: splice `count` copies of the pair `val` in at index `pos`. The value type
// HCEX_LIGHT carries a ref-counted weak-pointer handle, so each copy bumps the handle's ref count.
// When the vector already has capacity, the copies are made straight from `val`. When it must grow,
// `val` (which may alias the backing store Realloc frees) is first buffered into a temporary — the
// temporary takes its own reference — the copies are made from that buffer, and the temporary's
// reference is released at the end (freeing the handle if it was the last owner).
template<>
void dsVECTOR<dsPAIR<int, HCEX_LIGHT>, 8>::Insert(
        int pos, const dsPAIR<int, HCEX_LIGHT> &val, int count)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || count < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos, "nElem", this->nElem, "n", count);
    if (count == 0)
        return;

    if (this->allocated >= this->nElem + count) {
        // Spare capacity: copy each new pair directly from `val`.
        memmove(&this->pData[pos + count], &this->pData[pos],
                sizeof(dsPAIR<int, HCEX_LIGHT>) * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + count; ++slotIdx) {
            dsPAIR<int, HCEX_LIGHT> *slot = &this->pData[slotIdx];
            if (slot) {
                slot->key = val.key;
                slot->val.id = val.val.id;
                ds_WEAK_PTR_HANDLE *handle = val.val.ent.pHandle;
                slot->val.ent.pHandle = handle;
                if (handle)
                    osLockedIncrement(&handle->refCount);
                slot->val.entLight = val.val.entLight;
            }
        }
    } else {
        // Full: buffer `val` into a temporary (taking a reference) before reallocating.
        int tempKey = val.key;
        int tempId = val.val.id;
        ds_WEAK_PTR_HANDLE *tempHandle = val.val.ent.pHandle;
        if (tempHandle)
            osLockedIncrement(&tempHandle->refCount);
        entSFX_LIGHT *tempEntLight = val.val.entLight;

        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + count)
            newCapacity = this->nElem + count;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);

        memmove(&this->pData[pos + count], &this->pData[pos],
                sizeof(dsPAIR<int, HCEX_LIGHT>) * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + count; ++slotIdx) {
            dsPAIR<int, HCEX_LIGHT> *slot = &this->pData[slotIdx];
            if (slot) {
                slot->key = tempKey;
                slot->val.id = tempId;
                slot->val.ent.pHandle = tempHandle;
                if (tempHandle)
                    osLockedIncrement(&tempHandle->refCount);
                slot->val.entLight = tempEntLight;
            }
        }

        // Release the temporary's own reference (destructor of the buffered pair).
        if (tempHandle) {
            osLockedDecrement(&tempHandle->refCount);
            if (tempHandle->refCount <= 0) {
                operator delete(tempHandle);
                this->nElem += count;
                return;
            }
        }
    }
    this->nElem += count;
}
