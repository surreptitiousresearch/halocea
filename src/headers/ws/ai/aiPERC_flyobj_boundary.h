#pragma once
#include "../m3d/m3dV.h"

// ws-engine ai08 — deep collision/refine dependencies of aiPERC::ChoosePool, kept as a bounded
// boundary (the cdt/sr spatial-refine subsystem — srMANAGER::ShootCyl, aiREFINE_NAV_BUILD,
// cdtINFO_LSEG, cdtREFINE — is the next frontier). Declared as a single line-of-fire probe helper
// plus the jump-distance tunable so ChoosePool re-sources without dragging the whole cdt/sr graph.

struct aiBRAIN;

// The along-body-right sample distance used to seed the fly-object dodge probe (const float global
// jumpDist_0 @ 0x820DA7F0 in the ai08 TU).  boundary.
extern const float jumpDist_0;

// True when a cylinder cast (radius = brain's radCdt) from `from` to `to` hits geometry — i.e. the
// line of fire toward the dodge point is blocked. Wraps srMANAGER::ShootCyl(pSrMng, ...) against an
// aiREFINE_NAV_BUILD built from the brain's instance. boundary — body external to this batch.
bool aiPERC_FlyObjProbeBlocked(aiBRAIN *pBrain, const m3dV &from, const m3dV &to);
