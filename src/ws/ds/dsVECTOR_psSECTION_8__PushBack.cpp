#include "dsVECTOR.h"
#include "../ps/psSECTION.h"

// ps subsystem section payload. The 4-byte psSECTION handle aliases a psSECTION_DATA*; copying a
// handle bumps the payload's and its branch's refcounts. DB-verified offsets used below:
// psSECTION_DATA.refCount@0, psSECTION_DATA.pBranch@52; psSECTION_BRANCH.refCount@0. // boundary
struct psSECTION_BRANCH {
    int refCount; // 0x00
};
struct psSECTION_DATA {
    int               refCount;   // 0x00
    char              pad_04[48]; // 0x04..0x33
    psSECTION_BRANCH *pBranch;    // 0x34 (52)
    void Release();               // boundary — deferred release of the section payload
};

// dsVECTOR<psSECTION,8>::PushBack @ 0x8251BC40
// Append a copy of `val` and return the new last element. Copying a psSECTION shares its payload
// (psSECTION_DATA) and the payload's branch, bumping both refcounts. When the vector must grow, the
// payload is pre-referenced across the Realloc and the extra reference dropped via Release afterward.
template<>
psSECTION &dsVECTOR<psSECTION, 8>::PushBack(const psSECTION &val)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        psSECTION *slot = &this->pData[nElem];
        if (slot) {
            psSECTION_DATA *data = (psSECTION_DATA *)val.handle;
            if (data) {
                psSECTION_BRANCH *branch = data->pBranch;
                ++data->refCount;
                ++branch->refCount;
            }
            slot->handle = val.handle;
        }
    } else {
        psSECTION_DATA *data = (psSECTION_DATA *)val.handle;
        if (data) {
            psSECTION_BRANCH *branch = data->pBranch;
            ++data->refCount;
            ++branch->refCount;
        }
        int allocated = this->allocated;
        int newCapacity = 2 * allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > allocated)
            this->Realloc(newCapacity);

        psSECTION *slot = &this->pData[this->nElem];
        if (slot) {
            if (data) {
                psSECTION_BRANCH *branch = data->pBranch;
                ++data->refCount;
                ++branch->refCount;
            }
            slot->handle = (unsigned int)data;
        }
        if (data)
            data->Release(); // drop the reference taken across the Realloc
    }
    ++this->nElem;
    return this->Back();
}
