#pragma once
// ws-engine ai08: template-actor nav-system wrapper. DB-verified layout
// (types_members aiNAV_NS_WRAPPER_TPL) — size 0xC: aiNAV_NS_WRAPPER_GS@0 (8B base, opaque here),
// parentEntity@8. Constructed by aiPLANNER::CreateNavNSWrapperTpl.

#include "../ds/dsTSTRING.h"
#include "aiNAV_NS_WRAPPER_GS.h"

struct navMANAGER; // nav subsystem      boundary (ptr)
struct entENTITY;  // entENTITY.h

struct aiNAV_NS_WRAPPER_TPL : aiNAV_NS_WRAPPER_GS { // base 0x00..0x08 (DB-typed)
    entENTITY    *parentEntity;           // 0x08

    // 0x... — bind to `nm`'s nav graph for `ent`. boundary — body external to this batch.
    aiNAV_NS_WRAPPER_TPL(navMANAGER *nm, entENTITY *ent);

    // 0x8321AC38 — name for nav-system index `nsIdx`. This template specialisation returns the
    // empty string (a shared refcounted empty dsTSTRING buffer).
    dsTSTRING<char> GetName(int nsIdx) const;
};
