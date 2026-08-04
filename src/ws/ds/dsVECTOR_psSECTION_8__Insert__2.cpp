#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include <new> // placement new

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

// ps subsystem section payload; DB-verified refCount@0, pBranch@52 (see PushBack.cpp for the
// full layout comment).
struct psSECTION_BRANCH { int refCount; };
struct psSECTION_DATA {
    int               refCount;   // 0x00
    char              pad_04[48]; // 0x04..0x33
    psSECTION_BRANCH *pBranch;    // 0x34 (52)
};
// boundary — ps-subsystem section handle; copy-ctor bumps payload+branch refcounts, dtor releases.
struct psSECTION {
    psSECTION_DATA *pData;
    psSECTION(const psSECTION &other); // boundary
    ~psSECTION();                      // boundary
};

// dsVECTOR<psSECTION,8>::Insert(int, const psSECTION &, int) @ 0x8290EEC8 (fill overload)
// Splice `n` copies of the single element `val` in at index `pos`. When growing, `val` is first
// copy-constructed into a local temp (protecting it against aliasing the storage Realloc may move
// or free), then each new slot is copy-constructed from that temp; when no growth is needed each
// new slot is copy-constructed directly from `val`. Each copy bumps the shared payload's and its
// branch's refcounts (psSECTION's copy-ctor), mirroring PushBack/Insert(range)'s semantics.
template<>
void dsVECTOR<psSECTION, 8>::Insert(int pos, const psSECTION &val, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || n < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos,
            "nElem", this->nElem,
            "n", n);

    if (n == 0)
        return;

    if (this->allocated >= this->nElem + n) {
        memmove(&this->pData[pos + n], &this->pData[pos], 4 * (this->nElem - pos));
        for (int destIndex = pos; destIndex < pos + n; ++destIndex) {
            psSECTION *slot = &this->pData[destIndex];
            if (slot)
                new (slot) psSECTION(val);
        }
    } else {
        psSECTION valCopy(val); // protect against Realloc moving/freeing storage `val` may alias

        int neededSize = this->nElem + n;
        int doubledCapacity = 2 * this->allocated;
        int newCapacity = (doubledCapacity <= neededSize) ? neededSize : doubledCapacity;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);

        memmove(&this->pData[pos + n], &this->pData[pos], 4 * (this->nElem - pos));
        for (int destIndex = pos; destIndex < pos + n; ++destIndex) {
            psSECTION *slot = &this->pData[destIndex];
            if (slot)
                new (slot) psSECTION(valCopy);
        }
        // valCopy destructs here (~psSECTION), releasing the extra reference taken above.
    }
    this->nElem += n;
}
