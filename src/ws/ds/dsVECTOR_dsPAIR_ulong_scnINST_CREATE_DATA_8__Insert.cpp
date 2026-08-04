#include <new>
#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"
#include "../scn/scnINST_CREATE_DATA.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::Insert @ 0x823E2AE0
// Fill overload: splice `n` copies of the pair `val` in at index `pos`. The key is a trivial
// unsigned long, copied directly; the value is a non-trivial scnINST_CREATE_DATA, copy-constructed
// into each new slot. When the vector already has capacity, the copies are made straight from `val`.
// When it must grow, `val` (which may alias the backing store Realloc frees) is first copy-
// constructed into a stack temporary before Realloc; the copies are made from that temporary, which
// is destroyed at the end.
template<>
void dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::Insert(
        int pos, const dsPAIR<unsigned long, scnINST_CREATE_DATA> &val, int n)
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
        memmove(&this->pData[pos + n], &this->pData[pos], 92 * (nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + n; ++slotIdx) {
            dsPAIR<unsigned long, scnINST_CREATE_DATA> *slot = &this->pData[slotIdx];
            if (slot) {
                slot->key = val.key;
                new (&slot->val) scnINST_CREATE_DATA(val.val);
            }
        }
        this->nElem += n;
    } else {
        unsigned long tempKey = val.key;
        scnINST_CREATE_DATA temp(val.val);

        int allocated = this->allocated;
        int newCapacity = this->nElem + n;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        memmove(&this->pData[pos + n], &this->pData[pos], 92 * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + n; ++slotIdx) {
            dsPAIR<unsigned long, scnINST_CREATE_DATA> *slot = &this->pData[slotIdx];
            if (slot) {
                slot->key = tempKey;
                new (&slot->val) scnINST_CREATE_DATA(temp);
            }
        }
        temp.~scnINST_CREATE_DATA();
        this->nElem += n;
    }
}
