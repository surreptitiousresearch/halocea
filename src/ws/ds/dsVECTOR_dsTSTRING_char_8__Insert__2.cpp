#include <string.h>
#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsTSTRING<char>,8>::Insert @ 0x823DB6E0
// Range-insert `count` strings from `begin` at index `pos`: grow (doubling policy), shift the tail
// up, then copy-construct each new string. Copying a dsTSTRING shares the buffer header and bumps
// its refcount (emitted inline rather than through a ctor call).
template<>
void dsVECTOR<dsTSTRING<char>, 8>::Insert(
        int pos, const dsTSTRING<char> *begin, int count)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || count < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && num >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            382,
            "pos", pos, "nElem", this->nElem, "num", count);
    if (count == 0)
        return;

    int needed = this->nElem + count;
    dsTSTRING<char> *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < oldData + this->nElem)
        begin += this->pData - oldData;

    memmove(&this->pData[pos + count], &this->pData[pos], sizeof(dsTSTRING<char>) * (this->nElem - pos));
    for (int i = 0; i < count; ++i) {
        dsTSTRING<char> *slot = &this->pData[pos + i];
        slot->pBuffer = nullptr;                  // placement-init
        dsTSTRING_BUF_HEADER<char> *buf = begin[i].pBuffer;
        slot->pBuffer = buf;                      // share the buffer header
        ++buf->refCount;
    }

    this->nElem += count;
}
