#pragma once
#include "../ds/dsSHARED_PTR.h"
#include "../ds/Deleter.h"
// ws-engine fsm — hideout-transform info block held by a usePROCESSOR and returned by
// aiBODY::GetFsmInfo. DB-verified layout (types_members fsmHO_INFO, size 0x14).

struct mdlITRC_BASE; // mdl — interactive transform ref component   boundary (ptr in dsSHARED_PTR)

struct fsmHO_INFO {
    // DB nested enum fsmHO_INFO::SIDE — the hideout side; the DB carries no enumerator values, so
    // only the tag type is modelled (stored as a 4-byte field).  boundary.
    enum SIDE { FSM_HO_SIDE_NONE = 0 };

    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > itrcHo;     // 0x00 current hideout itrc
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > itrcCommon; // 0x08 common/fallback itrc
    SIDE side;                                                        // 0x10 hideout side

    // Default ctor — zero the shared pointers and side (?fsmHO_INFO@@QAA@XZ region).  boundary.
    fsmHO_INFO();
    // Compiler-generated copy assignment/copy-ctor (dsSHARED_PTR members handle refcounting) match
    // the DB's fsmHO_INFO::operator= at the aiBODY::GetFsmInfo call site.
};
