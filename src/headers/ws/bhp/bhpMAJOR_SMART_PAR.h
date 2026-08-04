#pragma once
#include "bhpPARAMS.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"

// ws-engine bhp subsystem — "major smart" behavior-parameter block, built by the planner when
// commanding an AI into a smart-object. DB-verified layout (types_members bhpMAJOR_SMART_PAR) —
// size 72 (0x48): bhpPARAMS@0 (40B base), then the smart-specific fields. The base is the canonical
// bhpPARAMS (typeBehav/notifyTaskSys etc. accessible) rather than an opaque span.

struct navGPH_CELL_DATA;   // nav — graph cell edge data     boundary (ptr)
struct smtSMART_OBJ_BASE;  // smt — smart-object base         boundary

struct bhpMAJOR_SMART_PAR : bhpPARAMS {
    const navGPH_CELL_DATA *data;         // 0x28
    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > smtObj; // 0x2C
    dsPAIR<dsSTRID, dsSTRID> *entryPointInfo; // 0x34
    float                    timeToExecute;   // 0x38
    bool                     isDelayedAttachAllowed; // 0x3C
    bool                     isInvited;       // 0x3D
    bool                     canInviteAI;     // 0x3E
    bool                     canInvitePlayer; // 0x3F
    dsTSTRING<char>          pathSpeed;       // 0x40
    bool                     isOK;            // 0x44
    // failNotifyAcceptorsSNum lives in the bhpPARAMS base (accessed as a dsVECTOR .pData in the
    // decompiler cleanup path); its exact base offset is a boundary — see note in the .cpp.

    // ctor(smtObj-ptr, entryPointInfo) / dtor defined elsewhere (bhp).  boundary
    // The dtor releases pathSpeed's buffer, Resets smtObj, restores the bhpPARAMS vtable, and frees
    // the base's failNotifyAcceptorsSNum vector (all inlined by the compiler at the call site in
    // a8_planner.cpp; encapsulated here as the destructor).
    bhpMAJOR_SMART_PAR(dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > *smt,
                       dsPAIR<dsSTRID, dsSTRID> *entryPointInfo);
    ~bhpMAJOR_SMART_PAR();
};
