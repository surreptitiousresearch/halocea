#pragma once
#include "aiNAV_EDGE_CUSTOM_DATA.h"
// ws-engine ai08 nav: one directed link in the navigation graph.
// DB-verified layout (types_members aiNAV_EDGE_DATA): idxFrom@0 (int), idxTo@4 (int),
// flags@8 (unsigned int), customData@C (aiNAV_EDGE_CUSTOM_DATA*), isDoubleLink@10 (bool) — size 20.

struct aiNAV_EDGE_DATA {
    int                     idxFrom;      // 0x00 source waypoint index
    int                     idxTo;        // 0x04 destination waypoint index
    unsigned int            flags;        // 0x08 edge flags
    aiNAV_EDGE_CUSTOM_DATA *customData;   // 0x0C optional attached attributes (null when none)
    bool                    isDoubleLink; // 0x10 true when the reverse edge also exists

    // 0x8318AD98 (?_RemoveData@aiNAV_EDGE_DATA@@AAAXXZ) — release and null out `customData`.
    void _RemoveData();
};
