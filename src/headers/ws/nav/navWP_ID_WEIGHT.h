#pragma once
// ws-engine nav: a waypoint id paired with a selection/scoring weight.
// DB-verified layout (types_members navWP_ID_WEIGHT): wpid@0 (short), weight@4 (float) — size 8.

struct navWP_ID_WEIGHT {
    short wpid;   // 0x00 waypoint id
    float weight; // 0x04 selection weight
};
