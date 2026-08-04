#include "psARRAY.h"
#include "psARRAY_DATA.h"
#include "../ds/dsDATA.h"
// 0x827AF3C0 — element `idx` boxed as a dsDATA; an empty dsDATA when the handle is null or `idx`
// is out of range. sret return: the decompiler swaps the sret slot with `this` (the real `this` is
// the array, spelled `result` there; `result->storage` == pData).

dsDATA psARRAY::GetElement(int idx) const
{
    dsDATA value;
    value.type = nullptr; // raw: only the type slot (offset 4) is zeroed for the empty case
    if (idx >= 0 && this->pData && idx < this->pData->data.nElem)
        value.StoreValue(this->pData->data[idx]);
    return value;
}
