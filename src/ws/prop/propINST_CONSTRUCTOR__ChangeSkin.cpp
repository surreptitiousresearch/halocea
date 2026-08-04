#include "../../headers/ws/prop/propINST_CONSTRUCTOR.h"
#include "../../headers/ws/ent/entENTITY.h"

extern void dlFree(void *ptr);

// 0x826E4750 — propINST_CONSTRUCTOR::ChangeSkin(const dsTSTRING<char>&)
// Adopts `presetName` as this instance's current skin, then delegates the actual instance
// rebuild to the shared instCONSTRUCTOR_MNG.
//
// Deviation from decompiler: the string assignment is the compiler INLINING
// dsTSTRING<char>::operator= rather than calling it — the disasm manipulates
// curPresetName.pBuffer/refCount directly with no `bl`. Reconstructed here as the equivalent
// explicit buffer release/adopt/refcount-bump instead of a decompiler-style opaque cast chain.
//
// Deviation from decompiler: the decompiler rendered the owner-instance lookup as
// `this->pOwner[1].propContainer.properties.__cl.file`, a garbage reinterpretation from a
// mistyped intermediate. The disasm is unambiguous: it reads a 4-byte pointer at
// propBASE::pOwner + 0x15C. propBASE::pOwner is declared iaIACTOR* in the DB, but
// DB-verified entENTITY (which derives from iaIACTOR at offset 0) places its `pInst`
// (animINST*) field at exactly offset 0x15C (348) — so pOwner is, in practice, always an
// entENTITY* here. Reconstructed as that base-to-derived cast plus a typed field access.

void propINST_CONSTRUCTOR::ChangeSkin(const dsTSTRING<char> &presetName)
{
    if (&presetName != &curPresetName)
    {
        dsTSTRING_BUF_HEADER<char> *oldBuffer = curPresetName.pBuffer;
        if (--oldBuffer->refCount == 0)
            dlFree(oldBuffer);

        dsTSTRING_BUF_HEADER<char> *newBuffer = presetName.pBuffer;
        curPresetName.pBuffer = newBuffer;
        ++newBuffer->refCount;
    }

    entENTITY *ownerEntity = (entENTITY *)pOwner; // see deviation note above
    mng.Construct(presetName, ownerEntity->pInst, false);
}
