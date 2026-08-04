#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ps/psSECTION_INTERFACE.h"
#include "../../headers/ws/ent/entDESC.h"
#include "../../headers/ws/ia/iaDESC.h"

// aiBRAIN_DESC::ParsePS @ 0x83186C90
// ?ParsePS@aiBRAIN_DESC@@UAAXVpsSECTION@@ABV?$dsTSTRING@D@@@Z
//
// Parse a brain descriptor: first the shared entDESC fields, fix the base scheduling priority,
// run the full Read* fan-out on the top-level section, then (if present) re-run the fan-out on the
// nested "AI" sub-section with an annotated error hint. `psRead` is taken by value (a referenced
// section copy) — its destruction at scope exit matches the decompiler's trailing dtor.
// The entDESC/iaDESC base region is modelled as an opaque span, so the base-class calls go through
// explicit reinterpret_casts of `this` (the base sits at offset 0).
void aiBRAIN_DESC::ParsePS(psSECTION psRead, const dsTSTRING<char> &hintErr)
{
    reinterpret_cast<entDESC *>(this)->ParsePS(psRead, hintErr);
    reinterpret_cast<iaDESC *>(this)->SetPriority(46);
    this->ReadAll(psRead, hintErr);

    const psSECTION_INTERFACE &ps = static_cast<const psSECTION_INTERFACE &>(psRead);
    psSECTION aiSection = ps.GetSection("AI", 0);
    if (aiSection.pData)
    {
        dsTSTRING<char> aiHint = hintErr + "(AI section)";
        this->ReadAll(aiSection, aiHint);
    }
}
