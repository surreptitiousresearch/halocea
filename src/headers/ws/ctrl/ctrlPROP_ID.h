#pragma once
// ws-engine ctrl: a property identifier — a 16-bit index into a unit's property/state bit cache.
// DB-verified layout (types_members ctrlPROP_ID): id@0 (unsigned __int16) — size 2.

struct dsSTRID; // ds/dsSTRID.h — interned string id (returned by GetStr)   boundary (fwd)

struct ctrlPROP_ID {
    unsigned short id; // 0x00 bit index into the ctrlPROP_LIST cache bit-array

    // ?GetStr@ctrlPROP_ID@@QBA?AVdsSTRID@@XZ — the interned string id for this property.
    // boundary — body external to this batch (reached by aiWATCHER::IsBigGun).
    dsSTRID GetStr() const;
};
