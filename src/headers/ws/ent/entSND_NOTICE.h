#pragma once
#include "../msg/msgDATA.h"
#include "../ia/iaIACTOR_SOUND.h" // iaSND_AI_VALUE
#include "../m3d/m3dV.h"
#include "../ds/dsTSTRING.h"

// ws-engine ent — a "sound heard" notice broadcast to nearby actors. DB-verified layout
// (types_members entSND_NOTICE) — size 0x24: msgDATA base @0, importance@4, volume@8, pos@0xC,
// nameEnt@0x18, nameEntDbg@0x1C, nameSnd@0x20.

struct entSND_NOTICE : msgDATA {
    iaSND_AI_VALUE  importance; // 0x04 AI importance bucket
    iaSND_AI_VALUE  volume;     // 0x08 loudness bucket (selects hearing radius)
    m3dV            pos;        // 0x0C world position of the sound
    dsTSTRING<char> nameEnt;    // 0x18 emitting entity name
    dsTSTRING<char> nameEntDbg; // 0x1C debug entity name
    dsTSTRING<char> nameSnd;    // 0x20 sound asset name
};
