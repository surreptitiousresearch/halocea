#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::Insert (range overload) @ 0x82F21A90
// Splice `num` pairs from the source range starting at `begin` in at index `pos`. If growing
// requires a Realloc and `begin` pointed inside the OLD backing store, `begin` is rebased onto the
// NEW store first (the decompiled `v21 += this->pData - pData` self-aliasing fixup) before copying
// each element (bumping both key and val buffer ref counts).
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::Insert(
        int pos, const dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *begin, int num)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || num < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && num >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            382,
            "pos", pos, "nElem", this->nElem, "num", num);
    if (num == 0)
        return;

    int allocated = this->allocated;
    int newCapacity = this->nElem + num;
    dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *oldData = this->pData;
    if (allocated < newCapacity) {
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);
    }

    // Rebase `begin` if it pointed inside the backing store and Realloc relocated it.
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    int destIdx = pos;
    memmove(&this->pData[pos + num], &this->pData[pos], 8 * (this->nElem - pos));
    for (int copied = num; copied; --copied, ++begin, ++destIdx) {
        dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *slot = &this->pData[destIdx];
        if (slot) {
            dsTSTRING_BUF_HEADER<char> *keyBuffer = begin->key.pBuffer;
            slot->key.pBuffer = keyBuffer;
            ++keyBuffer->refCount;
            dsTSTRING_BUF_HEADER<char> *valBuffer = begin->val.pBuffer;
            slot->val.pBuffer = valBuffer;
            ++valBuffer->refCount;
        }
    }
    this->nElem += num;
}
