#pragma once
#include "../m3d/m3dV.h"
#include "../ds/dsTSTRING.h"
// ws-engine gs: debug-primitive builder/initor. A process-global instance is filled in per
// draw call and handed to gsAddDebugPrimitive. DB-verified layout (headers_ref
// gsDEBUG_PRIMITIVE_INITOR) — size 0x34.

// dbgADD_PARAM — the additive-blend / replace mode selector (nested enum). Only ALPHA_REPLACE is
// referenced by this batch; kept as an int-backed enum so the value stays exact.
struct gsDEBUG_PRIMITIVE_INITOR {
    enum dbgADD_PARAM : int {
        REPLACE       = 0,
        ALPHA_REPLACE = 1,
        COLOR_REPLACE = 2,
        ADD           = 3,
        MULTIPLY      = 4,
    };

    m3dV            pos;      // 0x00
    m3dV            dir;      // 0x0C
    dsTSTRING<char> name;     // 0x18
    float           size;     // 0x1C
    int             type;     // 0x20
    float           time;     // 0x24
    unsigned int    color;    // 0x28
    float           viewDist; // 0x2C
    dbgADD_PARAM    addParam; // 0x30

    // 0x8256xxxx — ctor: zero-init + default add-param/name. boundary (body external to this batch).
    gsDEBUG_PRIMITIVE_INITOR();
};
