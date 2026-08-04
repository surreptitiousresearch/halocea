#include "psARRAY.h"
#include "psARRAY_DATA.h"
#include "../ds/dsDATA.h"
// 0x827AEA80 — box this array handle as a dsDATA value; an empty dsDATA when the handle is null.
// sret return: the decompiler swaps the sret slot with `this`.

dsDATA psARRAY::CreateObject()
{
    dsDATA boxed;
    boxed.type = nullptr; // raw: type slot (offset 4) zeroed before the (conditional) store
    if (this->pData)
        boxed.StoreValue<psARRAY>(*this);
    return boxed;
}
