#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT
extern "C" void dlFree(void *ptr);                                    // boundary — dlmalloc free

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::Erase @ 0x825CCB80
// Remove `n` 8-byte pairs starting at `pos`. Here the pair's value is a ref-counted dsTSTRING, so
// each erased element drops one reference on its value buffer; the leading int needs no teardown.
// The erased range is released first, then the tail is shifted down and the live count drops.
template<>
void dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::Erase(int pos, int n)
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
            dsTSTRING_BUF_HEADER<char> *valueBuffer = this->pData[eraseIndex].val.pBuffer;
            if (valueBuffer->refCount-- == 1) // last reference: release the buffer
                dlFree(valueBuffer);
        }
        memmove(&this->pData[pos], &this->pData[pos + n], 8 * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
