#pragma once
#include "../ent/entENTITY.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/PTR_LIST.h"
#include "grsBLADE_HIDE_REQ.h"
// ws-engine grass renderer entity. Derives from entENTITY; holds the per-unit grass state and
// the deferred hide/unhide request queue (hideRequest).
// DB-verified layout (types_members grsGRASS): base entENTITY@0 (444), then the members below.

// boundary — grass helper aggregates (not modeled in this batch).
struct grsUNIT_LIST;       /* UNRESOLVED: grsUNIT_LIST */
struct grsUNIT;            /* UNRESOLVED: grsUNIT */
struct grsGRASS_VSH_CONST; /* UNRESOLVED: grsGRASS_VSH_CONST */

typedef struct grsGRASS : entENTITY {                                          // 0x000 base (444 bytes)
    bool                                        collectGrsStat;                 // 0x1BC
    dsVECTOR<grsUNIT_LIST, 8>                   unitList;                       // 0x1C0
    ds::PTR_LIST<grsUNIT_LIST,
        ds::LIST_NODE_ACCESS_POLICY<grsUNIT_LIST> > unitVisList;               // 0x1D4
    dsVECTOR<dsTSTRING<char>, 8>                unitNames;                      // 0x1E0
    int                                         nextUnitUpdate;                 // 0x1F4
    dsVECTOR<grsBLADE_HIDE_REQ, 8>              hideRequest;                    // 0x1F8
    dsVECTOR<grsUNIT *, 8>                      obj2UnitList;                   // 0x20C
    dsVECTOR<grsGRASS_VSH_CONST, 16>            unitConsts;                     // 0x220
    float                                       rotAngle;                       // 0x234
    float                                       rendRotAngle;                   // 0x238
    int                                         isGrassLoaded;                  // 0x23C
    int                                         isGrassInited;                  // 0x240
    int                                         needFullUpdate;                 // 0x244
    bool                                        needVisUpdate;                  // 0x248
    dsVECTOR<int, 8>                            maxBlades;                      // 0x24C
    int                                         statNUnits;                     // 0x260
    int                                         statNObjRend;                   // 0x264
    int                                         statNUnitsAfterVisDom;          // 0x268
    int                                         statNUnitsFinal;                // 0x26C

    // ?UnhideAllGrass@grsGRASS@@QAAXXZ @ 0x82723830 — queue an unhide-all hide request.
    // REVERSED: src/ws/gfx/grsGRASS__UnhideAllGrass.cpp
    void UnhideAllGrass();
} grsGRASS;
