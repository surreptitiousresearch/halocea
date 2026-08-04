#include <string.h>
#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsTSTRING<char>,8>::Erase @ 0x825B4B50
// Remove `n` elements starting at `pos`: drop a buffer reference on each erased element first
// (freeing at zero), then shift the tail down over the gap.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Erase(int pos, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || n < 0 || pos + n > this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && n >= 0 && pos + n <= nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            417,
            "pos", pos, "nElem", this->nElem, "n", n);

    if (n == 0)
        return;

    for (int i = pos; i < pos + n; ++i) {
        dsTSTRING_BUF_HEADER<char> *buffer = this->pData[i].pBuffer;
        if (buffer->refCount-- == 1)
            dlFree(buffer);
    }
    memmove(&this->pData[pos], &this->pData[pos + n], sizeof(dsTSTRING<char>) * (this->nElem - pos - n));
    this->nElem -= n;
}
