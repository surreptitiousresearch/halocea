#pragma once
#include "../anim/animTPL.h"
// Boundary declarations for gsLVL_SYSTEM::FindTplName (0x827986D0): the scene template store it
// queries (scnSCENE / gsScenePtr) plus the scene-I/O and logging helpers. scnSCENE is a large
// engine object modeled here only up to pTplDummy@10220 (types_members scnSCENE) — the one field
// FindTplName reads directly; its template-management methods stay extern.

struct m3dV; // m3d — 3-vector (IsBelongPosSceneBox arg)  boundary (fwd)

typedef struct scnSCENE {
    unsigned char _pad0000[10220]; // 0x0000 — engine-internal scene state (not modeled here)
    animTPL      *pTplDummy;        // 0x27EC (10220) sentinel "template not found/loaded" marker

    // 0x827430B8 (?IsBelongPosSceneBox@scnSCENE@@QAAHPAUm3dV@@@Z) — nonzero when world position
    // `pos` lies inside the scene's bounding box. boundary.
    int IsBelongPosSceneBox(m3dV *pos);

    // Locate an already-resident template by (extension-stripped) name; returns pTplDummy on miss.
    animTPL *FindTplName(const char *name);
    // Allocate an empty template slot (null on exhaustion).
    animTPL *AllocTpl();
    // Release a template slot allocated via AllocTpl.
    void     DestroyTpl(animTPL *pTpl);
} scnSCENE;

extern scnSCENE *gsScenePtr;

// Install the scene-object load callback used while streaming a template in. boundary
void sioSetObjLoadCB(void (*cb)());

// Variadic logger. boundary (declared in several ws boundary headers).
void _apLog(const char *fmt, ...);

// Free a heap block (dsTSTRING buffer release path). boundary
extern void dlFree(void *ptr);
