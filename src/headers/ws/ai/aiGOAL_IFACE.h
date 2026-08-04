#pragma once
#include "aiGOAL_BASE.h"
// ws-engine ai08: interface layer between aiGOAL_BASE and the concrete aiGOAL.
// DB-verified layout (types_members aiGOAL_IFACE): base aiGOAL_BASE@0 (56), pBrain@0x38 — size 60.

struct aiBRAIN; // ai08 owning brain (fwd, pointer member)

struct aiGOAL_IFACE : aiGOAL_BASE {
    aiBRAIN *pBrain; // 0x38 owning brain
};
