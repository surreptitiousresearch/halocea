#pragma once
// ws-engine ai08 (a8_plan_chas.cpp) — externs/globals used by the aiCHASER re-sourced
// translation units. Layouts live in their canonical headers; this file only gathers the
// module-level symbols aiCHASER's methods reach for.

#include "aiCON_CB_globals.h"        // aiPLANNER *aiPlanner
#include "aiSUSTYPE.h"
#include "nsSUSCACHE.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"
#include "../wb/dbgVAR_boundary.h"   // dbgVAR_IMPL<bool,1>

struct navSYS;
struct entENTITY;
struct animINST;

// Module-global chaser suspicion caches (one per registered nav-system).
extern dsVECTOR<nsSUSCACHE, 8> susCache;

// (0,0,0) constant.
extern const m3dV m3dVZero;

// Current game time (seconds). ws module global (?t@@3MA). boundary.
extern float t;

// Debug colour palette (aRGB); index 8 used by the chaser debug renderer.
extern unsigned int Spectrum[];

// ai08 chaser debug con-var toggles (dbgVAR_IMPL<bool,1>; `.value`).
extern dbgVAR_IMPL<bool, 1> dbg_isChrLastPlr;
extern dbgVAR_IMPL<bool, 1> dbg_isChrMain;
extern dbgVAR_IMPL<bool, 1> dbg_isChrLastCorpse;
extern dbgVAR_IMPL<bool, 1> dbg_isChrLastShot;
extern dbgVAR_IMPL<bool, 1> dbg_isChrRandom;
extern dbgVAR_IMPL<bool, 1> dbg_isChrRealPlr;

// Player-slot access (ws gs layer). boundary.
int gsGetPlayerCount();
entENTITY *gsGetPlayer(int index);
