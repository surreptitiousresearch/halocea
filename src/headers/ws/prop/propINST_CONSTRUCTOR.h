#pragma once
#include "propENT.h"
#include "instCONSTRUCTOR_MNG.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsTYPE_ID.h"
// ws-engine prop subsystem: per-object "prop instance constructor" component — builds/configures
// a prop instance (e.g. a decorative/gameplay object with swappable skins) by delegating to the
// shared instCONSTRUCTOR_MNG (see instCONSTRUCTOR_MNG.h) with this object's currently-applied
// preset name.
// DB-verified layout (types_members propINST_CONSTRUCTOR):
//   propENT@0 (40B, base), curPresetName@40 (dsTSTRING<char>), mSeed@44 (unsigned int) — size 48.

// Descriptor for propINST_CONSTRUCTOR. Modeled here only far enough to expose its runtime-type
// bitmask static (?TYPE_ID@propINST_CONSTRUCTOR_DESC@@2VdsTYPE_ID@@A @ 0x8427C3BC), which
// propBASE::GetProperty<propINST_CONSTRUCTOR> matches against. Full layout is a boundary.
struct propINST_CONSTRUCTOR_DESC {
    static dsTYPE_ID TYPE_ID;
};

struct propINST_CONSTRUCTOR : propENT {
    dsTSTRING<char> curPresetName; // 0x28 preset/skin name currently applied to this instance
    unsigned int    mSeed;         // 0x2C

    // 0x826DD228 — number of presets registered with the shared instCONSTRUCTOR_MNG (reads
    // mng.presets.nElem directly; ignores `this` entirely).
    int GetPresetCount() const;

    // 0x826E19B8 — name of the preset at index `i`, taken from the shared preset tree
    // (mng.presets[i]->objName); ignores `this` entirely.
    const dsTSTRING<char> &GetPresetName(int i) const;

    // 0x826E4750 — apply `presetName` as this instance's current skin: adopt the new string
    // (inlined dsTSTRING assignment) unless it already aliases curPresetName, then delegate the
    // actual instance rebuild to the shared instCONSTRUCTOR_MNG::Construct.
    void ChangeSkin(const dsTSTRING<char> &presetName);
};
