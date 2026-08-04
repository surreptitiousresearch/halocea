#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ps/psSECTION.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// ps subsystem section payload; DB-verified refCount@0, pBranch@52 (see PushBack.cpp for the
// full layout comment).
struct psSECTION_BRANCH { int refCount; };
struct psSECTION_DATA {
    int               refCount;   // 0x00
    char              pad_04[48]; // 0x04..0x33
    psSECTION_BRANCH *pBranch;    // 0x34 (52)
};

// dsVECTOR<psSECTION,8>::Insert(int, const psSECTION *, int) @ 0x8251BE00 (pointer/range overload)
// Splice `num` elements from `begin` in at index `pos`: grow to nElem+num (doubling policy) if
// needed, re-basing `begin` if it aliased storage that Realloc moved, shift the tail up by `num`
// via memmove, then copy each incoming element in — each copy shares (and re-references) its
// source's psSECTION_DATA payload and that payload's branch, mirroring PushBack's copy semantics.
template<>
void dsVECTOR<psSECTION, 8>::Insert(int pos, const psSECTION *begin, int num)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || num < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && num >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            382,
            "pos", pos,
            "nElem", this->nElem,
            "num", num);

    if (num == 0)
        return;

    int allocated = this->allocated;
    int neededSize = this->nElem + num;
    psSECTION *oldData = this->pData;
    if (allocated < neededSize) {
        int newCapacity = neededSize;
        if (2 * allocated > neededSize)
            newCapacity = 2 * allocated;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);
    }

    // If `begin` aliased this vector's own storage, re-base it onto the (possibly moved) buffer.
    if (begin >= oldData && begin < &oldData[this->nElem])
        begin += this->pData - oldData;

    memmove(&this->pData[pos + num], &this->pData[pos], 4 * (this->nElem - pos));

    for (int destIndex = pos; destIndex < pos + num; ++destIndex, ++begin) {
        psSECTION *slot = &this->pData[destIndex];
        if (slot) {
            psSECTION_DATA *data = begin->pData;
            if (data) {
                psSECTION_BRANCH *branch = data->pBranch;
                ++data->refCount;
                ++branch->refCount;
            }
            slot->pData = begin->pData;
        }
    }
    this->nElem += num;
}
