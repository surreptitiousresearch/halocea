#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/dsSHARED_PTR.h"

// ws-engine prop subsystem — the "smart-object" property attached to an entity. The planner only
// reads smtObj, but its OFFSET is not a boundary: types_members propSMT gives propENT base@0x00
// (40), smtObj@0x28, pEntOwner@0x30, state@0x34 — size 56. This header used to put smtObj at 0,
// which reads into the propENT base instead.

#include "propENT.h"

struct entENTITY;
struct smtSMART_OBJ_BASE;

struct propSMT : propENT {
    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj; // 0x28
    entENTITY    *pEntOwner;     // 0x30
    apSTATE_T<unsigned char> state; // 0x34 (DB member type; 1 byte)
};
