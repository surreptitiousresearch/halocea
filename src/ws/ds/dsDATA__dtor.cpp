#include "dsDATA.h"
#include "dsDATA_TYPE.h"

// dsDATA::~dsDATA() @ 0x823B3F78
// Destroy the stored value when non-empty (dispatches the stored type's Destroy virtual, slot
// 0xC).
// DEVIATION: the decompiler rendered the virtual call as a bare function-pointer invocation with
// NO arguments (`((void(*)(void))type->Destroy)();`) -- a common decompiler artifact for
// const-qualified vtable dispatch losing its argument list. Every other reversed call to this
// same slot in this corpus (e.g. dsDATA::StoreValue<int>) passes `&storage`; reconstructed the
// same way here.
dsDATA::~dsDATA()
{
    const dsDATA_TYPE *storedType = this->type;
    if (storedType)
        storedType->Destroy(&this->storage);
}
