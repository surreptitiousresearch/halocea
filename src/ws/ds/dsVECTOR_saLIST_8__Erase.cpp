#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

extern "C" void *memmove(void *dst, const void *src, unsigned int n); // boundary — CRT

class msgADDR; // boundary — msg-system endpoint address

// boundary — sa/ds intrusive message-address list. DB layout is 20 bytes; only Clear() (destroy all
// nodes and reset to empty) is invoked by Erase.
namespace ds {
template<class T>
struct LIST {
    unsigned char _body[20]; // 0x00 opaque list head
    void Clear(); // boundary
};
}

// boundary — sa subsystem list element. DB-verified (types_members saLIST): msg@0 (int),
// addList@4 (ds::LIST<msgADDR*>, 20 bytes) — size 24.
struct saLIST {
    int                 msg;     // 0x00
    ds::LIST<msgADDR *> addList; // 0x04
};

// dsVECTOR<saLIST,8>::Erase @ 0x82AC62C0
// Remove `n` saLIST elements starting at index `pos`. Each element owns an addList whose nodes must
// be released first (inlined ds::LIST::Clear); then the tail is shifted down over the hole and the
// live count drops. (The saLIST body outside addList is trivially relocatable.)
template<>
void dsVECTOR<saLIST, 8>::Erase(int pos, int n)
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
        for (int eraseIndex = pos; eraseIndex < pos + n; ++eraseIndex)
            this->pData[eraseIndex].addList.Clear();
        memmove(&this->pData[pos], &this->pData[pos + n],
                sizeof(saLIST) * (this->nElem - pos - n));
        this->nElem -= n;
    }
}
