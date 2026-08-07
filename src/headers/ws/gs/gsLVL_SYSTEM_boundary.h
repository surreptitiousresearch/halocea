#pragma once
#include "../anim/animTPL.h"
#include "../scn/scnSCENE.h"
// Boundary declarations for gsLVL_SYSTEM::FindTplName (0x827986D0): the scene template store it
// queries (scnSCENE / gsScenePtr) plus the scene-I/O and logging helpers.
//
// DEVIATION: this header used to carry its own truncated scnSCENE body (`_pad0000[10220]` +
// pTplDummy, 10224 bytes) while src/headers/ws/scn/scnSCENE.h already carried the full
// DB-verified layout (types_members scnSCENE — 10352 bytes). Two bodies for one type is a latent
// C2011 and a second, contradictory layout; the canonical header is included instead, and the
// four template-store/bbox methods this file declared were moved onto it (all four DB-confirmed
// scnSCENE members: ?FindTplName@scnSCENE@@QAAPAVanimTPL@@PBD@Z @0x8253C6A8,
// ?AllocTpl@scnSCENE@@QAAPAVanimTPL@@XZ @0x8253EAA0,
// ?DestroyTpl@scnSCENE@@QAAXPAVanimTPL@@@Z @0x8253EBC8,
// ?IsBelongPosSceneBox@scnSCENE@@QAAHPAUm3dV@@@Z @0x827430B8).

extern scnSCENE *gsScenePtr;

// Install the scene-object load callback used while streaming a template in. boundary
void sioSetObjLoadCB(void (*cb)());

// Variadic logger. boundary (declared in several ws boundary headers).
void _apLog(const char *fmt, ...);

// Free a heap block (dsTSTRING buffer release path). boundary
extern "C" void dlFree(void *ptr);
