#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ssl/sslOBJ_REF.h"
#include "../ds/dsPAIR.h"

// ws-engine smt subsystem — smart-object base class. ref-verified layout
// (headers_ref smtSMART_OBJ_BASE.h) — size 0x34. Deep member types (weak-ptr holder, propSMT,
// desc) are boundary and kept as byte-accurate opaque placeholders. boundary.

struct smtSMART_OBJ_BASE_vtbl;
struct smtSMART_OBJ_GROUP;      // smt — owning group          boundary (ptr)
struct propSMT;                 // prop/propSMT.h              boundary (ptr)
struct smtSMART_OBJ_DESC_BASE;  // smt — descriptor base       boundary (ptr)
struct propUSABLE_SMT;          // prop — usable backlink       boundary (ptr)
struct entENTITY;               // ent/entENTITY.h             boundary (ptr)
struct dsVECTOR_ANY;            // used by GetAvailableSlotsInfo out-param (dsVECTOR<smtSMT_ENTER_INFO,8>)
struct propFSM;                 // prop — body FSM (Attach target)   boundary (ptr)

// sslCB_HOST base (first base) is defined in ai/aiBRAIN.h; represented here as an opaque leading
// word so this header stays standalone. mp::MSG_IFACE second base occupies the next word.
struct smtSMART_OBJ_BASE {
    smtSMART_OBJ_BASE_vtbl *__vftable;              // 0x00 sslCB_HOST::__vftable
    void                   *msgIfaceVtbl;           // 0x04 mp::MSG_IFACE::__vftable
    smtSMART_OBJ_GROUP     *grpOwner;               // 0x08
    sslOBJ_REF              sslObject;              // 0x0C
    dsSTRID                 name;                   // 0x10
    // ds::WEAK_PTR<entENTITY>: single pointer to a handle node holding the live entity pointer.
    struct WEAK_PTR_HANDLE_ent { entENTITY *pPtr; };
    struct { WEAK_PTR_HANDLE_ent *pHandle; } entHolder; // 0x14
    propSMT                *propSmt;                // 0x18
    const smtSMART_OBJ_DESC_BASE *desc;            // 0x1C
    int                     parentIdx;             // 0x20
    propUSABLE_SMT         *pUsableBacklink;       // 0x24
    int                     checkEnterActionFuncNameIdx; // 0x28
    int                     checkEnterFuncNameIdx;      // 0x2C
    int                     checkAvailabilityFuncNameIdx; // 0x30

    // Virtual: append the object's currently-available entry-point slots into `out`. boundary body.
    void GetAvailableSlotsInfo(void *out, int mode, void *cl);

    // Virtual ?Attach@smtSMART_OBJ_BASE@@UAAXPAVpropFSM@@PBV?$dsPAIR@VdsSTRID@@V1@@@_N@Z — attach the
    // body FSM `fsm` at entry action `action`; `isStreamingWait` requests a streaming-wait attach.
    // Called directly on the pointee at the aiBODY::AttachToSmart site.  boundary body.
    void Attach(propFSM *fsm, const dsPAIR<dsSTRID, dsSTRID> *action, bool isStreamingWait);

    // vtbl+0x58 — can `fsm` currently satisfy this object's usage condition? Used by aiBODY::CanUseSmart.
    // boundary — body external to this batch.
    bool CheckActionUsingCond(propFSM *fsm);
    // vtbl+0x48 — is the entry `action` available for `fsm` (mode flags in `flags`)? Used by
    // aiBODY::CanUseSmart.  boundary — body external to this batch.
    bool IsActionAvailable(const dsPAIR<dsSTRID, dsSTRID> *action, propFSM *fsm,
                           void *cl, unsigned int flags);
};
