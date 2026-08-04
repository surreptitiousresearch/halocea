#pragma once
#include "../ds/dsSHARED_PTR.h"
#include "../ds/Deleter.h"
#include "../ds/dsSTRID.h"
#include "../ds/MAP.h"
#include "aiWATCHER_BRAIN_boundaries.h" // canonical mdlITRC_BASE (IsHO/IsValid/GetSnapMatr/GetBaseMatr)
#include "../fsm/fsmEVENT_HANDLER.h"     // canonical fsmSTATE_MANAGER / fsmSTATE_FSM_HLD (fsm/fsmHld pointees)
// ws-engine ai08/fsm boundary — the minimal body-FSM query interface reached by
// aiWATCHER_PLAYER::GetHOMatrSnap / GetBodyPosNav. The propFSM state machine and its callback
// dispatch are the next re-source frontier; only the touched offsets/methods are modelled here as
// a BOUNDARY (do not treat as authoritative full layouts). fsmBODY_HO_INFO / fsmCB_HANDLER<dsSTRID>
// layouts are DB/headers_ref-verified; propFSM is a partial (eventHandler @0x3C only).

struct m3dMATR; // m3dMATR.h — 4x3 affine matrix   boundary (fwd)

// fsm callback param base + the pos/dir transport param — canonical single definition lives in
// fsm/fsmCB_POS_DIR_PAR.h (which also defines fsmCB_PARAM_BASE). Include it here so the two types
// have exactly one definition across every TU that reaches this boundary.
#include "../fsm/fsmCB_POS_DIR_PAR.h"

// Hideout-transform info returned by the body FSM's get_ho_info callback. DB-verified layout
// (headers_ref fsmBODY_HO_INFO) — size 0x18: three mdlITRC_BASE shared pointers.
struct fsmBODY_HO_INFO : fsmCB_PARAM_BASE {
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > itrc;       // 0x00 current HO itrc
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > itrcFrom;   // 0x08
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > itrcMpSafe; // 0x10

    // 0x83290AF4-region ctor — zero-init the three shared pointers. boundary.
    fsmBODY_HO_INFO();
};

struct fsmCB_HANDLER_dsSTRID_vtbl; // DB fsmCB_HANDLER<dsSTRID>_vtbl — boundary (fwd)
struct fsmMEM_CB_BASE;             // fsm member-callback node ({vtbl, id}) — pointer only (fwd)

// fsm event-callback handler keyed by interned event id — flattened spelling of the DB type
// fsmCB_HANDLER<dsSTRID>. DB-verified layout (types_members, size 0x5C): __vftable@0,
// cbMap@4 (ds::MAP<dsSTRID,fsmMEM_CB_BASE*,...>, 0x54), idLast@0x58. Only the entry points
// reached here are declared.  boundary.
struct fsmCB_HANDLER_dsSTRID {
    fsmCB_HANDLER_dsSTRID_vtbl *__vftable; // 0x00
    ds::MAP<dsSTRID, fsmMEM_CB_BASE *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> cbMap; // 0x04 event -> callback
    unsigned int                idLast;    // 0x58 last issued registration id

    // Dispatch the callback registered for `event`, passing `param` to be filled. Returns non-zero
    // when a callback was invoked. boundary — body external to this batch.
    int ActivateCB(const dsSTRID &event, fsmCB_PARAM_BASE *param);

    // Remove the callback that `obj` registered for `event`. boundary — body external to this batch.
    template<class T>
    void UnregisterCB(const dsSTRID &event, T *obj);
};

// prop FSM component (bodyFsm). DB-verified layout (types_members propFSM, size 0x99):
// propSYNCABLE base@0 (0x2C opaque), fsm@0x2C, fsmHld@0x34, eventHandler@0x3C, isFsmChangeDisabled@0x98.
struct propFSM {
    unsigned char _propSyncable[0x2C];       // 0x00 propSYNCABLE base (opaque boundary)
    dsSHARED_PTR<fsmSTATE_MANAGER, 0, Deleter<fsmSTATE_MANAGER> > fsm;    // 0x2C active state machine
    dsSHARED_PTR<fsmSTATE_FSM_HLD, 0, Deleter<fsmSTATE_FSM_HLD> > fsmHld; // 0x34 held (suspended) FSM
    fsmCB_HANDLER_dsSTRID eventHandler;      // 0x3C event-callback handler
    bool          isFsmChangeDisabled;       // 0x98
};

// A body-FSM "info system" object surfaced by the dbg_get_info_sys callback (wall-lean branch of
// GetBodyPosNav). Only the debug-info accessor reached there is modelled.  boundary.
struct fsmBODY_INFO_SYS_vtbl;
struct fsmBODY_INFO_SYS {
    fsmBODY_INFO_SYS_vtbl *__vftable; // 0x00
    // vtbl+0x94 — fill a debug-info block (>=0x3C bytes); the body world position is at out+0x30.
    void GetDbgInfo(void *out);
};

// Callback param that receives the info-sys pointer from dbg_get_info_sys.  boundary.
struct fsmBODY_INFO_SYS_PARAM : fsmCB_PARAM_BASE {
    fsmBODY_INFO_SYS *sys; // filled by the callback (null when unavailable)
};

// fsm body-FSM event id accessors (real statics). boundary — bodies external to this batch.
namespace fsmBODY_FSM_EVENT_events {
    // ?get_ho_info_@fsmBODY_FSM_EVENT@@SAABVdsSTRID@@XZ @0x829414A0
    const dsSTRID &get_ho_info();
    // ?dbg_get_info_sys_@fsmBODY_FSM_EVENT@@SAABVdsSTRID@@XZ @0x829F8B90
    const dsSTRID &dbg_get_info_sys();
}
