/* ?GetDbgName@sslOBJ_REF@@QBA?AV?$dsTSTRING@D@@XZ @0x825261F0 */
#include "sslOBJ_REF.h"
#include "sslOBJECT.h"

// 0x825261F0 — sslOBJ_REF::GetDbgName. Returns the referenced object's debug name by value (sret).
// The decompiler's `this`/`result` are swapped by the sret ABI (r3 = return slot, r4 = this):
// r4->pObject is read, and when null a sentinel string is built into the return slot.
dsTSTRING<char> sslOBJ_REF::GetDbgName() const
{
    if (pObject)
        return pObject->GetDbgName();

    // Empty ref: yield the sentinel "<<nullobj>>". The return slot begins uninitialised, so
    // UnsafeInit builds a fresh buffer (reserve = 0, len = -1 => whole string).
    dsTSTRING<char> dbgName;
    dbgName.pBuffer = nullptr;
    dbgName.UnsafeInit("<<nullobj>>", -1, 0);
    return dbgName;
}
