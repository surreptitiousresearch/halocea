#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"

struct mdlLIP_SYNCER; // boundary — mdl subsystem lip-sync animator (element holds a pointer)

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::Insert @ 0x823E2F08
// Fill overload: splice `n` copies of the pair `val` in at index `pos`. Both pair members (int key,
// raw pointer value) are trivial/POD, so this is a plain shift-then-copy with no refcounting.
// CAVEAT: the decompile packs (n,val) into a 64-bit register pair (__PAIR64__) purely as an ABI
// artifact of the two leading int-sized args; there is no real 64-bit value here — reconstructed as
// the plain two-argument call it represents.
template<>
void dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::Insert(
        int pos, const dsPAIR<int, mdlLIP_SYNCER *> &val, int n)
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
        memmove(&this->pData[pos + n], &this->pData[pos], 8 * (nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + n; ++slotIdx) {
            dsPAIR<int, mdlLIP_SYNCER *> *slot = &this->pData[slotIdx];
            if (slot) {
                slot->key = val.key;
                slot->val = val.val;
            }
        }
    } else {
        dsPAIR<int, mdlLIP_SYNCER *> temp = val; // snapshot before Realloc may move pData

        int allocated = this->allocated;
        int newCapacity = this->nElem + n;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        memmove(&this->pData[pos + n], &this->pData[pos], 8 * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + n; ++slotIdx) {
            dsPAIR<int, mdlLIP_SYNCER *> *slot = &this->pData[slotIdx];
            if (slot)
                *slot = temp;
        }
    }
    this->nElem += n;
}
