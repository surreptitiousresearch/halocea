#pragma once
#include "bhpPARAMS.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"

// ws-engine bhp subsystem — smart-object behavior-parameter block.
// ref-verified layout (headers_ref bhpSMART_PAR.h) — size 0x3C.

struct smtSMART_OBJ_BASE;   // smt/smtSMART_OBJ_BASE.h  boundary (ptr in dsSHARED_PTR)

struct bhpSMART_PAR : bhpPARAMS {
    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj; // 0x28
    dsPAIR<dsSTRID, dsSTRID> entryPointInfo;   // 0x30
    float                    timeToExecute;    // 0x38

    bhpSMART_PAR();
    bhpSMART_PAR(dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > *smt,
                 dsPAIR<dsSTRID, dsSTRID> *entryPointInfo);
};
