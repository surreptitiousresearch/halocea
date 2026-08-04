#pragma once
#include "../ds/dsSHARED_PTR.h"

// ws-engine prop subsystem — the "smart-object" property attached to an entity. Only the smtObj
// shared pointer is needed by the planner; the rest of propSMT (and the propBASE/propCONTAINER
// traversal used to locate it) is a boundary (next frontier).

struct smtSMART_OBJ_BASE;

struct propSMT {
    // Offset within propSMT is a boundary; the planner only ever reads this via a found propSMT*.
    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj;
};
