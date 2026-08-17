/* ?GetOwner@psSECTION_INTERFACE@@QBA?AVpsSECTION@@XZ @0x8251ABF8 */
#include "psSECTION_INTERFACE.h"
#include "psSECTION.h"
#include "psSECTION_DATA.h"
#include "psSECTION_BRANCH.h"
// 0x8251ABF8 — the owner section handle (empty when this handle is null or has no owner). sret
// return: the decompiler swaps the sret slot with `this`.
//
// CAVEAT: the AddRef on the owner body/branch is emitted TWICE by the compiler (an inlined
// by-value psSECTION copy-construct followed by a second reference bump), then the result is stored
// and the extra reference released once — net +1 reference, matching a normal handle copy. The two
// AddRefs are reproduced faithfully below.

psSECTION psSECTION_INTERFACE::GetOwner() const
{
    psSECTION result;
    if (this->pData) {
        psSECTION_DATA *owner = this->pData->pOwner;
        if (owner) {
            ++owner->refCount;
            ++owner->pBranch->refCount;
            ++owner->refCount;
            ++owner->pBranch->refCount;
        }
        result.pData = owner;
        if (owner)
            owner->Release();
    } else {
        result.pData = nullptr;
    }
    return result;
}
