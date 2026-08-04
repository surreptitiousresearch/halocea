#pragma once

// ws-engine ai08 — a "watcher holder": pairs an aiWATCHER with a reference counter.
// DB-verified layout (types_members aiWTC_HOLDER) — size 8.

struct aiWATCHER; // aiWATCHER.h — the held watcher   boundary (ptr)
struct iaIACTOR;  // ia subsystem — interactive actor   boundary (fwd)

struct aiWTC_HOLDER {
    aiWATCHER *watcher; // 0x00 the held watcher
    int        counter; // 0x04 reference/use counter

    // Construct a holder for `actor` (creates the watcher via aiWATCHER::Create). ai08 boundary.
    aiWTC_HOLDER(iaIACTOR *actor);

    // Destroy the held watcher. ai08 boundary (non-trivial; body external to this batch).
    ~aiWTC_HOLDER();

    // 0x8316E740 — true when the held watcher's UID equals `uid`.
    bool HasUID(unsigned int uid) const;
};
