#include "psSECTION_INTERFACE.h"
#include "../ds/dsDATA.h"
// 0x82517008 — box this section handle as a dsDATA value; an empty dsDATA when the handle is null.
// sret return: the decompiler swaps the sret slot with `this`.

dsDATA psSECTION_INTERFACE::CreateObject() const
{
    dsDATA boxed;
    boxed.type = nullptr;
    if (this->pData)
        boxed.StoreValue<psSECTION>(*this);
    return boxed;
}
