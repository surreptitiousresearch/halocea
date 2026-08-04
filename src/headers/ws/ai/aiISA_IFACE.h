#pragma once
#include "aiISA_BASE.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08: aiISA_IFACE — an aiISA_BASE (interop/social-awareness mind module) bound to a
// specific brain. DB (types_members aiISA_IFACE, size 0x8): aiISA_BASE base @0x00, pBrain @0x04.

struct aiBRAIN; // ai08 — per-unit AI brain (full def aiBRAIN.h)   boundary (ptr)

struct aiISA_IFACE : aiISA_BASE {
    aiBRAIN *pBrain; // 0x04 owning brain

    // 0x8329FDD8 (?SetPar@aiISA_IFACE@@UAA_NABV?$dsTSTRING@D@@M@Z) — virtual: apply the shared
    // "viewPercAlert" tunable to the status block; true if consumed. REVERSED:
    // src/ws/ai/aiISA_IFACE__SetPar.cpp.
    bool SetPar(const dsTSTRING<char> &key, float val);
};
