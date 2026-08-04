#pragma once
#include "../m3d/m3dRECT.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "fntLETTER.h"
// ws-engine ui_new: the debug-stat bitmap font — a glyph atlas (sorted letter table keyed by
// character code) plus its overall line-height bounding box.
// DB-verified layout (types_members ui_new::UI_STAT_FONT): bbox@0 (m3dRECT, 16B), m_Letters@16
// (dsVECTOR<fntLETTER,8>, 20B), m_TextureName@36 (dsTSTRING<char>), m_Name@40 (dsTSTRING<char>),
// m_Initialized@44 (int) — size 48.

namespace ui_new {

typedef struct UI_STAT_FONT {
    // Compare functor passed to m_Letters.FindSorted: matches a fntLETTER against a raw
    // character-code key (used by UI_STAT_SYS::PrintLine). Not reversed in this batch. boundary.
    struct CmpFindLetter {
        int cmp(const fntLETTER &letter, const int &symbol) const; // boundary
    };

    m3dRECT               bbox;          // 0x00 overall glyph bounding box (l/t/r/b), used as the
                                            //      per-line advance/height by UI_STAT_SYS
    dsVECTOR<fntLETTER, 8> m_Letters;     // 0x10 sorted by fntLETTER::symbol
    dsTSTRING<char>        m_TextureName; // 0x24 glyph-atlas texture name
    dsTSTRING<char>        m_Name;        // 0x28
    int                    m_Initialized; // 0x2C
} UI_STAT_FONT;

} // namespace ui_new
