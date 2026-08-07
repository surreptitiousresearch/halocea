#pragma once
#include "../ds/dsFLAGS.h"       // stateUser is dsFLAGS<ITRC_ST,int>
#include "../ds/dsSHARED_PTR.h"  // vtbl slots 20/24 traffic in dsSHARED_PTR<mdlITRC_BASE,...>

// ws-engine mdl: base of the "interactive transform reference component" (ITRC) — the runtime
// handle a body/AI holds onto a moving attach point (mount seat, hideout corner, sync target).
// DB-verified layout (types_members mdlITRC_BASE): __vftable@0, stateUser@4 — size 8. The concrete
// 33-slot vtable is DB-verified (types_members mdlITRC_BASE_vtbl); pointer params are forward-declared
// boundary types (the next frontier). Only the slots this drain actually calls carry inline wrappers.

#include "ITRC_ST.h"      // mdl — per-itrc state flags (DB types_enum_values)
#include "MP_ITRC_TYPE.h" // mdl — multiplayer itrc kind (DB types_enum_values)
// mdl — hideout corner position kind.
// DB-verified: types_enum_values ITRC_HO_POS_TYPE carries all four names and values verbatim.
// The previous body was INFERRED and wrong, under a comment asserting "the DB carries the tag but
// no enumerator values" — a negative claim the enum oracle contradicts outright. It invented
// ITRC_HO_POS_NONE for the DB's ITRC_HO_POS_UNDEF, omitted MIDDLE entirely, and therefore had
// RIGHT one short at 2. Found 2026-08-06 by find_enum_values.py, the first check to compare a
// header enumerator's VALUE against the binary at all.
enum ITRC_HO_POS_TYPE {
    ITRC_HO_POS_UNDEF  = 0,
    ITRC_HO_POS_LEFT   = 1,
    ITRC_HO_POS_MIDDLE = 2,
    ITRC_HO_POS_RIGHT  = 3,
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

struct mdlITRC_BASE_vtbl;

struct mdlITRC_BASE {
    // Nested boundary type: DB types_members mdlITRC_BASE::UPDATE_INFO (trIn/mInst/mPivot/mShape/
    // idObjShape/idObjPivot/dt/typeMove/offRotTurn/syncInfo/vUp/isHardPlacement/rideSpeed/
    // scaleUser). Declared incomplete -- only Update's slot parameter names it here. It has to be
    // declared inside the class, so the class body precedes the vtable and the inline wrappers
    // below move out of line.
    struct UPDATE_INFO;

    mdlITRC_BASE_vtbl    *__vftable;   // 0x00
    dsFLAGS<ITRC_ST, int> stateUser;   // 0x04

    // Thin wrappers over the DB-verified vtable slots the HO drain dispatches through.
    bool IsHO();                       // slot 9
    bool IsValid();                    // slot 8
    bool GetSnapMatr(m3dMATR *m);      // slot 3
    bool GetBaseMatr(m3dMATR *m);      // slot 5
    ITRC_HO_POS_TYPE GetCornerPosType(); // slot 30
};

// DB-verified 33-slot vtable (types_members mdlITRC_BASE_vtbl).
struct mdlITRC_BASE_vtbl {
    void   (*dtr_mdlITRC_BASE)(mdlITRC_BASE *self, int deleteFlag);                                  // 0 — deleting dtor: vftable+0x00 holds ??_GmdlITRC_BASE@@UAAPAXI@Z
    void   (*Update)(mdlITRC_BASE *self, const mdlITRC_BASE::UPDATE_INFO *updateInfo, m3dTR *); // 1
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

inline bool mdlITRC_BASE::IsHO()                  { return __vftable->IsHO(this); }
inline bool mdlITRC_BASE::IsValid()               { return __vftable->IsValid(this); }
inline bool mdlITRC_BASE::GetSnapMatr(m3dMATR *m) { return __vftable->GetSnapMatr(this, m); }
inline bool mdlITRC_BASE::GetBaseMatr(m3dMATR *m) { return __vftable->GetBaseMatr(this, m); }
inline ITRC_HO_POS_TYPE mdlITRC_BASE::GetCornerPosType() { return __vftable->GetCornerPosType(this); }
