#pragma once
#include "../ds/dsSTRID.h"
// ws-engine dmg subsystem: a scoped damage-logging helper. Captures the animation instance, the
// starting health and a name filter at construction; Start() begins recording a damage event.
// DB-verified layout (types_members dmgLOG): __vftable@0, pInst@4, healthStart@8, nameFilter@0xC.

struct animINST;   // ws/anim — animation instance (full def animINST.h)  boundary (ptr)
struct dmgDAMAGE;  // ws/dmg — damage record                              boundary (ptr)
struct dmgLOG_vtbl;

struct dmgLOG {
    dmgLOG_vtbl *__vftable;   // 0x00
    animINST    *pInst;       // 0x04 animation instance being logged
    int          healthStart; // 0x08 health at start of the event
    dsSTRID      nameFilter;  // 0x0C damage-name filter id

    // ctor (??0dmgLOG@@QAA@PAVanimINST@@HABVdsSTRID@@@Z) — capture inst/health/filter.  boundary.
    dmgLOG(animINST *pInst, int healthStart, const dsSTRID &nameFilter);
    // 0x82FD3508 (?Start@dmgLOG@@QAAXPBVdmgDAMAGE@@@Z) — begin recording `pDmg`.  boundary.
    void Start(const dmgDAMAGE *pDmg);
    // ~dmgLOG (?1dmgLOG@@UAA@XZ) — flush/close the log entry.  boundary (virtual).
    ~dmgLOG();
};
