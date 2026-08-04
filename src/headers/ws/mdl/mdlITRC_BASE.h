#pragma once
#include "../ds/dsFLAGS.h"       // stateUser is dsFLAGS<ITRC_ST,int>
#include "../ds/dsSHARED_PTR.h"  // vtbl slots 20/24 traffic in dsSHARED_PTR<mdlITRC_BASE,...>

// ws-engine mdl: base of the "interactive transform reference component" (ITRC) — the runtime
// handle a body/AI holds onto a moving attach point (mount seat, hideout corner, sync target).
// DB-verified layout (types_members mdlITRC_BASE): __vftable@0, stateUser@4 — size 8. The concrete
// 33-slot vtable is DB-verified (types_members mdlITRC_BASE_vtbl); pointer params are forward-declared
// boundary types (the next frontier). Only the slots this drain actually calls carry inline wrappers.

enum ITRC_ST;             // mdl — per-itrc state flags (boundary)
enum MP_ITRC_TYPE;        // mdl — multiplayer itrc kind (boundary)
// mdl — hideout corner position kind. The DB carries the tag but no enumerator values; the two
// corner tags reached by aiBODY::SetZeroGravity are modelled with INFERRED (unverified) values
// (only equality against LEFT/RIGHT is used, so ordering is not load-bearing).
enum ITRC_HO_POS_TYPE {
    ITRC_HO_POS_NONE  = 0,
    ITRC_HO_POS_LEFT  = 1,
    ITRC_HO_POS_RIGHT = 2,
};

struct mdlITRC_BASE;
struct m3dV;
struct m3dTR;
struct m3dMATR;
struct entTRACKER;
struct entENTITY;
struct objOBJ;
struct dsSTRID;
struct propBODY_FSM;
struct ctrlPROP_LIST;

// DB-verified 33-slot vtable (types_members mdlITRC_BASE_vtbl).
struct mdlITRC_BASE_vtbl {
    void   (*dtr_mdlITRC_BASE)(mdlITRC_BASE *self);                                  // 0
    void   (*Update)(mdlITRC_BASE *self, const void *updateInfo, m3dTR *);           // 1  updateInfo = mdlITRC_BASE::UPDATE_INFO*
    unsigned int (*PredictMovement)(mdlITRC_BASE *self, const m3dV *);               // 2
    bool   (*GetSnapMatr)(mdlITRC_BASE *self, m3dMATR *);                            // 3
    bool   (*GetSnapTrk)(mdlITRC_BASE *self, entTRACKER *);                          // 4
    bool   (*GetBaseMatr)(mdlITRC_BASE *self, m3dMATR *);                            // 5
    bool   (*IsReached)(mdlITRC_BASE *self);                                         // 6
    bool   (*FailedToReach)(mdlITRC_BASE *self);                                     // 7
    bool   (*IsValid)(mdlITRC_BASE *self);                                           // 8
    bool   (*IsHO)(mdlITRC_BASE *self);                                              // 9
    bool   (*IsSync)(mdlITRC_BASE *self);                                            // 10
    entENTITY *(*GetSnapEnt)(mdlITRC_BASE *self);                                    // 11
    objOBJ    *(*GetSnapObj)(mdlITRC_BASE *self);                                    // 12
    void   (*Reset)(mdlITRC_BASE *self);                                             // 13
    void   (*Lock)(mdlITRC_BASE *self);                                              // 14
    void   (*UnLock)(mdlITRC_BASE *self, const m3dMATR *);                           // 15
    bool   (*IsLocked)(mdlITRC_BASE *self);                                          // 16
    void   (*Freeze)(mdlITRC_BASE *self, bool);                                      // 17
    void   (*SetSpecialUpdate)(mdlITRC_BASE *self, bool, bool);                      // 18
    void   (*SyncWithPoint)(mdlITRC_BASE *self, const m3dV *);                       // 19
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > *
           (*GetHOItrc)(mdlITRC_BASE *self,
                        dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > *result); // 20
    void   (*MarkAsCanBeLocked)(mdlITRC_BASE *self, bool);                           // 21
    bool   (*IsCanAttack)(mdlITRC_BASE *self);                                       // 22
    bool   (*DecideUpdateSync)(mdlITRC_BASE *self);                                  // 23
    bool   (*ReattachInternalHo)(mdlITRC_BASE *self,
                        const dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > *); // 24
    void   (*OnAttach)(mdlITRC_BASE *self, bool);                                    // 25
    void   (*OnShake)(mdlITRC_BASE *self);                                           // 26
    bool   (*IsItrcRiding)(mdlITRC_BASE *self);                                      // 27
    MP_ITRC_TYPE (*GetMpType)(mdlITRC_BASE *self);                                   // 28
    dsSTRID *(*GetType)(mdlITRC_BASE *self, dsSTRID *result);                        // 29
    ITRC_HO_POS_TYPE (*GetCornerPosType)(mdlITRC_BASE *self);                        // 30
    void   (*UpdatePredictionProps)(mdlITRC_BASE *self, const propBODY_FSM *, ctrlPROP_LIST *); // 31
    void   (*UpdatePredictionPropsAI)(mdlITRC_BASE *self, ctrlPROP_LIST *);          // 32
};

struct mdlITRC_BASE {
    mdlITRC_BASE_vtbl    *__vftable;   // 0x00
    dsFLAGS<ITRC_ST, int> stateUser;   // 0x04

    // Thin wrappers over the DB-verified vtable slots the HO drain dispatches through.
    bool IsHO()                   { return __vftable->IsHO(this); }        // slot 9
    bool IsValid()                { return __vftable->IsValid(this); }     // slot 8
    bool GetSnapMatr(m3dMATR *m)  { return __vftable->GetSnapMatr(this, m); } // slot 3
    bool GetBaseMatr(m3dMATR *m)  { return __vftable->GetBaseMatr(this, m); } // slot 5
    ITRC_HO_POS_TYPE GetCornerPosType() { return __vftable->GetCornerPosType(this); } // slot 30
};
