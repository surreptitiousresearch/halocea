#pragma once
#include "aiREFINE_TRAJ_COMMON.h"
// ws-engine ai08 — far-range trajectory-refine filter. DB-verified (types_members
// aiREFINE_TRAJ_FAR, size 56): an aiREFINE_TRAJ_COMMON base with no additional members
// (own vtable/ctor only).

struct aiREFINE_TRAJ_FAR : aiREFINE_TRAJ_COMMON {
    // ??0aiREFINE_TRAJ_FAR@@QAA@ABUaiPARAMS_RFN_TRAJ@@@Z — boundary.
    aiREFINE_TRAJ_FAR(const aiPARAMS_RFN_TRAJ &params);
    // ??1aiREFINE_TRAJ_FAR@@UAA@XZ — boundary.
    ~aiREFINE_TRAJ_FAR();
};
