#include "iaIACTOR.h"
#include "../sml/STATE.h"
#include "../ps/psSECTION_INTERFACE.h"

// iaIACTOR::UnshareDescIfNeeded(psSECTION, const dsTSTRING<char>&) — 0x826D8450
// Clone this actor's descriptor to a private copy (via UnshareDescByInst) before applying
// `section`'s config, unless both (a) `section` and its parent hierarchy have no keys to apply
// (IsEmptyHier), and (b) the owning propCONTAINER_DESC says no instance-level unshare is needed
// either — in which case sharing the descriptor is safe and nothing is cloned.
void iaIACTOR::UnshareDescIfNeeded(psSECTION section, const dsTSTRING<char> &hintErr)
{
    // Deviation from decompiler: it typed the by-value `section` parameter itself as
    // `psSECTION_INTERFACE *` — an artifact of `section` arriving as an invisible reference and
    // being used directly as `this` for IsEmptyHier. DB-verified psSECTION_INTERFACE adds no data
    // of its own over psSECTION (just methods), so the two are layout-identical and the
    // reinterpret below is exact, not a guess.
    const psSECTION_INTERFACE &sectionView = (const psSECTION_INTERFACE &)section;

    if (!sectionView.IsEmptyHier() ||
        Desc<iaIACTOR>(this)->propContainerDesc.pointee->IsNeedToUnshareByInst())
    {
        // `section` is passed by value again here (same invisible-reference-by-value convention
        // as this function's own parameter), so the compiler-generated copy constructor call the
        // decompiler showed explicitly (a fresh psSECTION built from `section`) happens implicitly.
        UnshareDescByInst(section, hintErr);
    }
}
