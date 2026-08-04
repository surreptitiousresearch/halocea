#pragma once
#include "../cdt/cdtREFINE.h"
#include "../prop/propGAME_INFO_TEAM.h"
// ws-engine ai08 — shared base of the trajectory-refine filters (aiREFINE_TRAJ_NEAR/_FAR).
// DB-verified layout (types_members aiREFINE_TRAJ_COMMON, size 56 / 0x38):
//   cdtREFINE base@0 (0x20), isInclPlayer@0x20, teamPeers@0x24 (propGAME_INFO_TEAM, 8),
//   isThroughGlass@0x2C, pInstTarget@0x30, pInstWeapon@0x34.

struct aiPARAMS_RFN_TRAJ; // ai08 refine-parameter block — the next frontier (fwd)

struct aiREFINE_TRAJ_COMMON : cdtREFINE {
    int                 isInclPlayer;   // 0x20
    propGAME_INFO_TEAM  teamPeers;      // 0x24
    bool                isThroughGlass; // 0x2C
    animINST           *pInstTarget;    // 0x30
    animINST           *pInstWeapon;    // 0x34

    // ??0aiREFINE_TRAJ_COMMON@@IAA@HKABUaiPARAMS_RFN_TRAJ@@@Z — protected ctor. boundary.
    // ??1aiREFINE_TRAJ_COMMON@@UAA@XZ — dtor (virtual via cdtREFINE). boundary.
    ~aiREFINE_TRAJ_COMMON();
    // ?IsInclPlayer@aiREFINE_TRAJ_COMMON@@QAA_NXZ / ?IsIncludedManual@... — boundary.
    bool IsInclPlayer();
    bool IsIncludedManual(animINST *pInst);

protected:
    aiREFINE_TRAJ_COMMON(int isInclPlayer, unsigned long flags, const aiPARAMS_RFN_TRAJ &params);
};
