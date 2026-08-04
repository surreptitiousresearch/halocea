#pragma once
#include "aiBEST_BASE.h"
// ws-engine ai08: adds the owning-brain back-pointer between aiBEST_BASE and the concrete aiBEST.
// DB-verified layout (types_members aiBEST_IFACE): aiBEST_BASE base@0, pBrain@4 — size 8.

struct aiBRAIN; // ai08 — owning brain (fwd)

struct aiBEST_IFACE : aiBEST_BASE {
    aiBRAIN *pBrain; // 0x04 owning brain
};
