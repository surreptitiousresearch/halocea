#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc free

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::Insert (fill overload) @ 0x823E27E0
// Splice `n` copies of the pair `val` in at index `pos`. Both key and val are ref-counted string
// handles, so each copy bumps both buffers' ref counts. When the vector already has capacity, the
// copies are made straight from `val`. When it must grow, `val` (which may alias the backing store
// Realloc frees) is first buffered (its own references taken) before Realloc; the copies are made
// from that buffer, and the temporary's references are released at the end (val released before
// key, matching the decompiled order; an early return after freeing the key buffer skips the
// trailing `nElem += n`, folded into that branch below).
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::Insert(
        int pos, const dsPAIR<dsTSTRING<char>, dsTSTRING<char> > &val, int n)
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
            dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *slot = &this->pData[slotIdx];
            if (slot) {
                dsTSTRING_BUF_HEADER<char> *keyBuffer = val.key.pBuffer;
                slot->key.pBuffer = keyBuffer;
                ++keyBuffer->refCount;
                dsTSTRING_BUF_HEADER<char> *valBuffer = val.val.pBuffer;
                slot->val.pBuffer = valBuffer;
                ++valBuffer->refCount;
            }
        }
        this->nElem += n;
    } else {
        dsTSTRING_BUF_HEADER<char> *tempKeyBuffer = val.key.pBuffer;
        ++tempKeyBuffer->refCount;
        dsTSTRING_BUF_HEADER<char> *tempValBuffer = val.val.pBuffer;
        ++tempValBuffer->refCount;

        int allocated = this->allocated;
        int newCapacity = this->nElem + n;
        if (2 * allocated > newCapacity)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        memmove(&this->pData[pos + n], &this->pData[pos], 8 * (this->nElem - pos));
        for (int slotIdx = pos; slotIdx < pos + n; ++slotIdx) {
            dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *slot = &this->pData[slotIdx];
            if (slot) {
                slot->key.pBuffer = tempKeyBuffer;
                ++tempKeyBuffer->refCount;
                slot->val.pBuffer = tempValBuffer;
                ++tempValBuffer->refCount;
            }
        }

        // Release the temporary's own references (destructor of the buffered pair), val before key.
        if (tempValBuffer->refCount-- == 1)
            dlFree(tempValBuffer);
        if (tempKeyBuffer->refCount-- == 1) {
            dlFree(tempKeyBuffer);
            this->nElem += n;
            return;
        }
        this->nElem += n;
    }
}
