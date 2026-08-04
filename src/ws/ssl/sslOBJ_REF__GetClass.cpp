#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
#include "sslCLASS_REF.h"

// 0x82524370 — sslOBJ_REF::GetClass. Returns the referenced object's class as a fresh sslCLASS_REF
// by value (sret). The decompiler swaps this/result (r3 = return slot, r4 = this): the return slot's
// pClass is cleared, then when this ref is non-empty _SetClassPtr adopts pObject->refClass.pClass.
sslCLASS_REF sslOBJ_REF::GetClass() const
{
    sslCLASS_REF classRef;
    classRef.pClass = nullptr;
    if (pObject)
        classRef._SetClassPtr(pObject->refClass.pClass);
    return classRef;
}
