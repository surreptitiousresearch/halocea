/* ?GetParent@sslCLASS_REF@@QBA?AV1@XZ @0x82523980 */
#include "sslCLASS_REF.h"
#include "sslCLASS.h"

// 0x82523980 — sslCLASS_REF::GetParent. The parent (base) class reference of the referenced class,
// returned by value (sret). The decompiler swaps this/result (r3 = return slot, r4 = this): when
// the ref is empty the result is an empty ref, otherwise it adopts refParent via _SetClassPtr. The
// ref-count fixups are carried by _SetClassPtr and the return copy/temporary teardown.
sslCLASS_REF sslCLASS_REF::GetParent() const
{
    if (this->pClass)
    {
        sslCLASS_REF parentRef;
        parentRef.pClass = nullptr;
        parentRef._SetClassPtr(this->pClass->refParent.pClass);
        return parentRef;
    }
    return sslCLASS_REF();
}
