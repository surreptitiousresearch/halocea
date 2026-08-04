#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_EFFECT_LOC.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// dsVECTOR<HCEX_EFFECT_LOC,8>::Insert @ 0x823E0410  (range overload: const HCEX_EFFECT_LOC*, num)
// Splice `num` effect locations from `begin` in at index `pos`: grow (doubling policy), rebase
// `begin` if it aliased storage Realloc moved, shift the tail up, then copy-construct each new slot.
// Copying an HCEX_EFFECT_LOC is memberwise: the two float3s (pos, vec) are bit-copied and the `name`
// dsTSTRING shares the source buffer header (refcount bump), emitted inline rather than via a ctor.
template<>
void dsVECTOR<HCEX_EFFECT_LOC, 8>::Insert(int pos, const HCEX_EFFECT_LOC *begin, int num)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || num < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && num >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            382,
            "pos", pos, "nElem", this->nElem, "num", num);
    if (num == 0)
        return;

    int needed = this->nElem + num;
    HCEX_EFFECT_LOC *oldData = this->pData;
    if (this->allocated < needed) {
        if (2 * this->allocated > needed)
            needed = 2 * this->allocated;
        if (needed > this->allocated)
            this->Realloc(needed);
    }
    if (begin >= oldData && begin < &oldData[this->nElem])
        begin += this->pData - oldData;

    memmove(&this->pData[pos + num], &this->pData[pos],
            sizeof(HCEX_EFFECT_LOC) * (this->nElem - pos));
    for (int i = 0; i < num; ++i) {
        HCEX_EFFECT_LOC *slot = &this->pData[pos + i];
        slot->pos = begin[i].pos;                 // bit-copy the float3 position
        slot->vec = begin[i].vec;                 // bit-copy the float3 direction
        slot->name.pBuffer = nullptr;             // placement-init the string
        dsTSTRING_BUF_HEADER<char> *buf = begin[i].name.pBuffer;
        slot->name.pBuffer = buf;                 // share the name buffer header
        ++buf->refCount;
    }

    this->nElem += num;
}
