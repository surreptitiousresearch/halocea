#pragma once
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "iaSND_SOURCE_DATA.h"
// ws-engine ia sound subsystem: one playing-sound record. DB-verified layout (types_members
// iaSND_PLAY_DATA) — size 40: pBuff@0, paramIndex@4, pSound@8, sndDesc@12, nextSndDesc@16,
// srcData@20 (iaSND_SOURCE_DATA, 20B). snd::BUFFER (FMOD) and iaSOUND/iaSND_DESC stay
// pointer-only boundaries.

namespace snd { struct BUFFER; } // snd — FMOD-backed sound buffer (boundary)
struct iaSOUND;                  // ia — sound descriptor (boundary)
struct iaSND_DESC;               // ia — refcounted sound description (boundary)

struct iaSND_PLAY_DATA {
    snd::BUFFER   *pBuff;      // 0x00
    int            paramIndex; // 0x04
    const iaSOUND *pSound;     // 0x08
    dsSMART_PTR<iaSND_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > sndDesc;     // 0x0C
    dsSMART_PTR<iaSND_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > nextSndDesc; // 0x10
    iaSND_SOURCE_DATA srcData; // 0x14

    // 0x82A4ECD8 (?GetTotalTime@iaSND_PLAY_DATA@@QBAHXZ) — total duration of the sound, in
    // milliseconds. boundary.
    int GetTotalTime() const;

    // 0x82A4ED00 (?SetRPCParameter@iaSND_PLAY_DATA@@QAAXPBDM@Z) — set the named FMOD RPC parameter
    // (`name`) on the playing sound to `value`. boundary.
    void SetRPCParameter(const char *name, float value);
};
