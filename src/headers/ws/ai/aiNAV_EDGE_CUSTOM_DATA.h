#pragma once
#include "../ds/dsTSTRING.h"
// ws-engine ai08 nav: optional per-edge attributes attached to a navigation edge.
// DB-verified layout (types_members aiNAV_EDGE_CUSTOM_DATA): smartObjName@0 (dsTSTRING<char>),
// lenMul@4 (float) — size 8.

struct aiNAV_EDGE_CUSTOM_DATA {
    dsTSTRING<char> smartObjName; // 0x0 name of an attached smart-object (empty when none)
    float           lenMul;       // 0x4 traversal-length multiplier applied to this edge
};
