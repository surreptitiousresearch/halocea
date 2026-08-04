#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc free

// dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8>::Insert @ 0x823B7D80
// Fill overload: splice `n` copies of the pair `val` in at index `pos`. The key type dsTSTRING<char>
// is a ref-counted string handle, so each copy bumps the buffer's ref count; the trailing int value
// is copied directly. When the vector already has capacity, the copies are made straight from `val`.
// When it must grow, `val` (which may alias the backing store Realloc frees) is first buffered (its
// own reference taken) before Realloc, the copies are made from that buffer, and the temporary's
// reference is released at the end (freeing the buffer if it was the last owner — in which case the
// decompiled early-return skips the trailing `nElem += n`, so it is folded into the freeing branch).
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, int>, 8>::Insert(
        int pos, const dsPAIR<dsTSTRING<char>, int> &val, int n)
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
            dsPAIR<dsTSTRING<char>, int> *slot = &this->pData[slotIdx];
            if (slot) {
                dsTSTRING_BUF_HEADER<char> *buffer = val.key.pBuffer;
                slot->key.pBuffer = buffer;
                ++buffer->refCount;
                slot->val = val.val;
            }
        }
    } else {
        dsTSTRING_BUF_HEADER<char> *tempBuffer = val.key.pBuffer;
        ++tempBuffer->refCount;
        int tempVal = val.val;

        int allocated = this->allocated;
        int newCapacity = this->nElem + n;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        memmove(&this->pData[pos + n], &this->pData[pos], 8 * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + n; ++slotIdx) {
            dsPAIR<dsTSTRING<char>, int> *slot = &this->pData[slotIdx];
            if (slot) {
                slot->key.pBuffer = tempBuffer;
                ++tempBuffer->refCount;
                slot->val = tempVal;
            }
        }

        // Release the temporary's own reference (destructor of the buffered pair).
        if (tempBuffer->refCount-- == 1) {
            dlFree(tempBuffer);
            this->nElem += n;
            return;
        }
    }
    this->nElem += n;
}
