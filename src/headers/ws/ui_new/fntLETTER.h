#pragma once
#include "../m3d/m3dRECT.h"
// ws-engine ui_new: one glyph in a UI_STAT_FONT's letter table — its character code plus the
// screen-space quad (poly) and texture-atlas quad (uv) used to draw it.
// DB-verified layout (types_members ui_new::fntLETTER): symbol@0 (int), poly@4 (m3dRECT, 16B),
// uv@20 (m3dRECT, 16B) — size 36.

namespace ui_new {

typedef struct fntLETTER {
    int      symbol; // 0x00 character code this glyph represents
    m3dRECT  poly;    // 0x04 unscaled screen-space quad (relative to the pen position)
    m3dRECT  uv;      // 0x14 texture-atlas UV quad
} fntLETTER;

} // namespace ui_new
