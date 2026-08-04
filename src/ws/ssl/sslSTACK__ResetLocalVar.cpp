#include "sslSTACK.h"
#include "../ds/dsDATA.h"
#include "../ds/dsDATA_TYPE.h"

// boundary — the process-wide descriptor for the null/empty value type
// (dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType).
struct NULL_TYPE;
template<class T> struct dsDATA_TYPE_STORAGE { static const dsDATA_TYPE *dataType; };

// 0x82AF4590 — sslSTACK::ResetLocalVar. Ensure local `idx` exists (growing the locals vector),
// set its declared type to `pType` (defaulting to the null type), then reset its stored value to a
// freshly-constructed default of that type.
void sslSTACK::ResetLocalVar(const dsDATA_TYPE *pType, int idx)
{
    if (idx + 1 > this->localsStack.nElem)
        this->localsStack.Resize(idx + 1);

    LOCAL_VAR &local = this->localsStack[idx];
    local.pType = pType;

    const dsDATA_TYPE *effType = pType ? pType : dsDATA_TYPE_STORAGE<NULL_TYPE>::dataType;
    dsDATA scratch;
    scratch.type = effType;
    effType->Construct(&scratch.storage);
    local.val.StoreValue(scratch);
    if (scratch.type)
        scratch.type->Destroy(&scratch.storage);
}
