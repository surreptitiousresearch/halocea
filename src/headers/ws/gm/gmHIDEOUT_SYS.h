#pragma once
#include "gmHO_CTRL_BASE.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/Deleter.h"
#include "../ho/RAY_RELATED_ERROR.h"
#include "../ia/iaIACTOR.h"
// ws-engine gm: the global hideout subsystem + its debugger base. DB-verified sizes
// (types_members gmHIDEOUT_DEBUGGER 0xAC, gmHIDEOUT_SYS = gmHIDEOUT_DEBUGGER + iaIACTOR, 0x174).
// Only the query entry points this drain dispatches through are declared. Bodies external (gm).

struct cdtHO_PARAMS;
struct gmHO_CTRL_BASE;
struct mdlITRC_HO;
struct hoEDGE;
enum HO_CHANGE_TYPE;
struct hoOBJECT; // ho — hideout object (fwd, boundary; GetPointsNearBody arg)
struct propFSM;  // prop — per-body FSM state (fwd, boundary; GetPointsNearBody args)

struct gmHIDEOUT_DEBUGGER_vtbl;

// DB-verified layout (types_members gmHIDEOUT_DEBUGGER) — size 0xAC: __vftable@0,
// lastErrorSearch@4, lastErrorSelect@0x58 (RAY_RELATED_ERROR x2). Used as a scratch error
// collector on failed enter/search. Explicit __vftable + no virtual methods per convention.
struct gmHIDEOUT_DEBUGGER {
    gmHIDEOUT_DEBUGGER_vtbl *__vftable;       // 0x00
    RAY_RELATED_ERROR        lastErrorSearch; // 0x04
    RAY_RELATED_ERROR        lastErrorSelect; // 0x58

    gmHIDEOUT_DEBUGGER();   // boundary
    ~gmHIDEOUT_DEBUGGER();  // boundary
    // Format the most recent recorded error into a message string. Body external.
    const char *GetLastError(bool full); // boundary
};

// DB-verified size 0x174: gmHIDEOUT_DEBUGGER base@0 (0xAC) + iaIACTOR base@0xAC (200B),
// modeled as multiple inheritance per the DB's two anonymous base rows.
struct gmHIDEOUT_SYS : gmHIDEOUT_DEBUGGER, iaIACTOR {
    // Try to enter the hideout described by `params`; returns the live controller or nullptr.
    gmHO_CTRL_BASE *EnterHideout(const cdtHO_PARAMS *params); // boundary
    // Change from `prev` HO onto `edgeNew` under `params`; returns the new controller or nullptr.
    gmHO_CTRL_BASE *ChangeHideout_2(const cdtHO_PARAMS *params, gmHO_CTRL_BASE *prev,
                                    hoEDGE *edgeNew, HO_CHANGE_TYPE *changeType); // boundary
    // Wrap a controller into a fresh HO itrc handle. Body external.
    mdlITRC_HO *NewItrcHo_2(gmHO_CTRL_BASE *ctrl); // boundary

    // 0x82FCA180 (?GetPointsNearBody@gmHIDEOUT_SYS@@UAAX...) — virtual: collect the hideout ITRC
    // points near the body identified by (`fsmTarget`, `fsmBody`) for `hideout` into `out`;
    // `preferCenter` biases toward centre slots. Used by aiBODY::GetHOPointsNearBody.  boundary.
    void GetPointsNearBody(hoOBJECT *hideout, propFSM *fsmTarget, propFSM *fsmBody,
                           dsVECTOR<dsSHARED_PTR<mdlITRC_HO, 0, Deleter<mdlITRC_HO> >, 8> &out,
                           bool preferCenter); // boundary
};

extern gmHIDEOUT_SYS *gmHideOutSys; // global hideout subsystem singleton
