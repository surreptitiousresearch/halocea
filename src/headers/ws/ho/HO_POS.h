#pragma once
// ws-engine ho: an edge + position-along-edge pair. DB-verified (types_members HO_POS), size 8.

struct hoEDGE;

struct HO_POS {
    hoEDGE *edge; // 0x00
    float   pos;  // 0x04
};
