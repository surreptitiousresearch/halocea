#pragma once
#include "aiVIS_POINTS.h"
// ws-engine ai08: the pending enemy-visibility probe (the 4 sampled sight points plus a "real"
// flag). DB-verified layout (headers_ref aiVIS_QUERY) — size 0x64, align 4.

struct aiVIS_QUERY {
    aiVIS_POINTS points; // 0x00 sampled sight points for the probe
    bool         isReal; // 0x60 a genuine (non-sentinel) query is pending
    // 0x61..0x63 padding
};
