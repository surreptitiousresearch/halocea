/* ?GetName@sslOBJ_REF@@QBA?AV?$dsTSTRING@D@@XZ @0x825248E8 */
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"

// 0x825248E8 — sslOBJ_REF::GetName. Returns the referenced object's name as a fresh dsTSTRING<char>
// by value (sret), sharing the object's name buffer (bumping its refCount). When the ref is empty
// the shared empty-string singleton is used. The decompiler swaps this/result (r3 = return slot
// dsTSTRING*, r4 = this sslOBJ_REF*).
dsTSTRING<char> sslOBJ_REF::GetName() const
{
    dsTSTRING<char> name;
    if (this->pObject)
    {
        // sslOBJECT::name (offset 4) — share its buffer header.
        name.pBuffer = this->pObject->name.pBuffer;
    }
    else
    {
        name.pBuffer = nullptr;
        name.UnsafeInitEmpty();
    }
    ++name.pBuffer->refCount;
    return name;
}
