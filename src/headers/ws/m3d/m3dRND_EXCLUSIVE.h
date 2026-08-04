#pragma once
// ws-engine m3d: a random index generator that avoids repeating the last-drawn value.
// DB-verified layout (types_members m3dRND_EXCLUSIVE): topIdx@0 (int), lastIdx@4 (int) — size 8.

struct m3dRND_EXCLUSIVE {
    int topIdx;  // 0x0 exclusive upper bound of the index range
    int lastIdx; // 0x4 last value drawn (excluded from the next draw)

    // Set the range [0, top) and reset the last-drawn value.  boundary
    void Init(int top);

    // 0x82563230 (?Get@m3dRND_EXCLUSIVE@@QAAHXZ) — draw a fresh index in [0, topIdx) that differs
    // from lastIdx; updates lastIdx. boundary (defined elsewhere).
    int Get();
};
