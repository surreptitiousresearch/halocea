#pragma once
#include "../ctrl/ctrlPROP_ID.h"
// Shared boundary declarations for the aiBRAIN / aiTIMER_POOL batch: globals and cross-subsystem
// free functions reversed in their own subsystems.

struct propGAME_INFO; // prop subsystem — per-actor game info   boundary (ptr)
struct propFSM;       // prop subsystem — per-body FSM state     boundary (ptr)
struct m3dV;          // ws m3d — 3-vector

// dl heap free. boundary (unmangled C export; extern "C" matches aiDEBUG_boundaries.h).
extern "C" void dlFree(void *ptr);

// ap logging (variadic ~AI~ channel). boundary (?_apLog@@YAXPBDZZ — C++ linkage).
extern void _apLog(const char *fmt, ...);

// 0x82C3C2F8 (?m3dUpdateTimeField@@YAHPAMM@Z) — count `*field` down by `dt`, clamping at 0;
// returns nonzero when it reached/passed zero this step. boundary.
extern int m3dUpdateTimeField(float *field, float dt);

// 0x842D64A0 — the "weapon-line" state-property id, indexed into a body's prop bit-cache. boundary.
// const to match the canonical ctrlPROP_ID extern in aiWATCHER_props_boundary.h (same global).
extern const ctrlPROP_ID MDL_ST_PROP_WLN;

// 0x8317A6E8 (?aiGetBodyCS@@YAXPAVpropGAME_INFO@@PAVpropFSM@@_NAAUm3dV@@33@Z) — compute a body's
// world-space coordinate system (position/forward/up) from its game info and FSM, honouring the
// weapon-line flag. boundary.
extern void aiGetBodyCS(propGAME_INFO *gameInfo, propFSM *fsm, bool weaponLine,
                        m3dV &pos, m3dV &dir, m3dV &dirUp);
