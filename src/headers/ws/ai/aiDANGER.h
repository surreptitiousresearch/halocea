#pragma once
// ws-engine ai08: an active danger the brain is reacting to (base of grenade/other danger types).
// DB-verified partial layout (types_members aiDANGER): __vftable@0, type@4. The grenade-danger
// subclass carries the tracked grenade's multiplayer/instance id at 0x08 (read by
// aiBRAIN::implAvoidDanger to look the grenade up in aiGRASS); modeled here as `grenadeMPID`.

// ai08 — danger discriminator (DB types_enum_values AI_DANGER_TYPE). Only the grenade kind is
// enumerated in the DB; other kinds (if any) are a boundary.
enum AI_DANGER_TYPE {
    AI_DNG_GRENADE = 0,
};
struct aiDANGER_vtbl;

struct aiDANGER {
    aiDANGER_vtbl        *__vftable;    // 0x00
    const AI_DANGER_TYPE  type;         // 0x04 danger kind
    int                   grenadeMPID;  // 0x08 grenade instance id (grenade-danger subclass field)
};
