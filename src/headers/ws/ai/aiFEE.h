#pragma once
// ws-engine ai08 — "FEE" = a foe/entity evaluation record produced by aiWAH_SYS::PutAllFEEs.
// DB-verified layout (types_members aiFEE) — size 16 (0x10).

struct aiWTC_HOLDER; // aiWTC_HOLDER.h
struct aiBRAIN;      // ai08 — per-unit AI brain (viewer) boundary (fwd)

struct aiFEE {
    float        dist;     // 0x00 distance to target
    float        dist4cmp; // 0x04 squared/comparison distance
    bool         isInOpen; // 0x08 target is in the open
    bool         isPlayer; // 0x09 target is a player
    // 0x0A..0x0B padding
    aiWTC_HOLDER *hold;    // 0x0C source holder

    aiFEE();
    ~aiFEE();

    // Recompute this record from `hold`'s watcher. ai08 boundary.
    void Reset(aiWTC_HOLDER *hold);
    // True when this record represents a usable target. ai08 boundary.
    bool IsValid() const;

    // 0x8321FD38 — recompute dist/dist4cmp for a melee viewer: distance from the viewer's
    // reference point to this holder's body, weighted by the viewer descriptor's melee coefficients
    // (player vs non-player).
    void CalculateDistanceForMelee(const aiBRAIN *viewer);
    // 0x8321FE20 — recompute dist/dist4cmp for a ranged viewer, additionally recording whether the
    // target is in the open, and weighting by the open/player descriptor coefficients.
    void CalculateDistanceForRanged(const aiBRAIN *viewer);
};
