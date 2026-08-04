#pragma once
#include "aiSND_BASE.h"
// ws-engine ai08: inserts the owning-brain back-pointer between aiSND_BASE and the concrete aiSND.
// DB-verified layout (types_members aiSND_IFACE): aiSND_BASE base@0, pBrain@4 — size 8.

struct aiBRAIN; // ai08 — owning brain (fwd)

struct aiSND_IFACE : aiSND_BASE {
    aiBRAIN *pBrain; // 0x04 owning brain
};
