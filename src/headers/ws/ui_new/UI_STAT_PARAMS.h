#pragma once
#include "../m3d/m3dCOLOR.h"
// ws-engine ui_new: text-rendering parameters for one printed stat line/box (color, alignment,
// scale, per-character interval, drop-shadow flag).
// DB-verified layout (types_members ui_new::UI_STAT_PARAMS): color@0 (m3dCOLOR, 16B),
// alignment@16 (int), scale@20 (float), interval@24 (float), hasShadow@28 (bool) — size 32.

namespace ui_new {

typedef struct UI_STAT_PARAMS {
    m3dCOLOR color;      // 0x00
    int      alignment;  // 0x10 — horizontal bits in 0x07 (0x02=right?/0x04=center, see
                           //        UI_STAT_SYS::AdjustStringPos), vertical bits in 0x70
                           //        (0x20/0x40, see UI_STAT_SYS::GetPosFromViewport)
    float    scale;      // 0x14
    float    interval;   // 0x18 extra per-character horizontal spacing
    bool     hasShadow;  // 0x1C
} UI_STAT_PARAMS;

} // namespace ui_new
