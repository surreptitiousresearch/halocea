#pragma once
#include "../ds/dsSHARED_PTR.h"   // dsSHARED_PTR + Deleter
#include "../ds/dsTSTRING.h"

// ws-engine ai — free helpers in the `AI` static-method holder used by the aiPLANNER death-smt-object
// registration path. Body external (next frontier). boundary.

struct iaIACTOR;          // ia/iaIACTOR.h                boundary (fwd)
struct smtSMART_OBJ_BASE; // smt subsystem — smart-object base  boundary (fwd)

struct AI {
    // 0x833075A0 (?GetSmtObjFromActor@AI@@SA_NPAViaIACTOR@@AAV?$dsSHARED_PTR@VsmtSMART_OBJ_BASE@@...@@AAV?$dsTSTRING@D@@@Z)
    // Resolve the smart-object owned by `actor` into `smtObj`; returns false and fills `errHint` with a
    // reason string on failure. boundary.
    static bool GetSmtObjFromActor(iaIACTOR *actor,
                                   dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > &smtObj,
                                   dsTSTRING<char> &errHint);
};
