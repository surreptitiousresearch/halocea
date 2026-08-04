#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/hcex_light.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" long osLockedDecrement(volatile int *value);              // boundary — atomic decrement
void operator delete(void *p);                                       // boundary — global operator delete

// dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8>::Erase @ 0x823DAA0 (fn 0x823DA9F0)
// Remove `n` elements starting at `pos`. Each pair's HCEX_LIGHT holds a ds_WEAK_PTR to the bound
// entity; erasing releases that weak reference (atomic decref, delete the shared handle at zero) and
// nulls the handle. Then the 16-byte elements are shifted down over the gap and the count dropped.
template<>
void dsVECTOR<dsPAIR<int, HCEX_LIGHT>, 8>::Erase(int pos, int n)
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
            dsPAIR<int, HCEX_LIGHT> *pair = &this->pData[eraseIndex];
            ds_WEAK_PTR_HANDLE *handle = pair->val.ent.pHandle;
            if (handle) {
                osLockedDecrement(&handle->refCount);
                if (handle->refCount <= 0) {
                    ds_WEAK_PTR_HANDLE *dead = pair->val.ent.pHandle;
                    if (dead)
                        operator delete(dead);
                }
            }
            pair->val.ent.pHandle = nullptr;
        }
        memmove(&this->pData[pos], &this->pData[pos + n], 16 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
