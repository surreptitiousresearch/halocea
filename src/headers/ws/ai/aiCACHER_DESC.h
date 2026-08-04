#pragma once
#include "../ia/iaDESC.h"
#include "../ps/psSECTION.h"
#include "../ds/dsTSTRING.h"
// ws-engine ai08: descriptor for the AI "cacher" actor. Adds no fields of its own over iaDESC
// (DB-verified: types_members aiCACHER_DESC size 0xA4 == iaDESC size) — it exists only to override
// ParsePS with a distinct default priority.

struct aiCACHER_DESC : iaDESC {
    // 0x83163A58 (virtual) — parse the shared iaDESC config, then force priority 40.
    // REVERSED: src/ws/ai/aiCACHER_DESC__ParsePS.cpp.
    void ParsePS(psSECTION ps, const dsTSTRING<char> &hintErr);
};
