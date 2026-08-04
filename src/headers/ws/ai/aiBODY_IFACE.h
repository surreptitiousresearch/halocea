#pragma once
// ws-engine ai08: the body interface an aiBRAIN drives (locomotion/FSM/props). Its full layout and
// 37-slot vtable (DB type aiBODY_IFACE_vtbl) are the NEXT FRONTIER; this batch (aiBRAIN::GetBodyCS)
// reaches only two virtuals, dispatched by their DB-verified vtable slot index rather than
// fabricating the 34 preceding slot declarations. Faithful to the disassembly.

#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsSTRID.h"
#include "../ctrl/ctrlPROP_LIST.h" // canonical ctrlPROP_LIST (GetRefPropCache return)

struct smtSMART_OBJ_BASE; // smt/smtSMART_OBJ_BASE.h — smart object   boundary (ptr in dsSHARED_PTR)
struct propFSM;       // prop subsystem — per-body FSM state    boundary (ptr)
struct entENTITY;     // ent subsystem — game entity            boundary (ptr)
struct m3dV;          // ws m3d — 3-vector                       boundary (ptr)
struct m3dMATR;       // ws m3d — 4x3 transform                  boundary (ptr)
struct aiBRAIN;       // ai08 — owning brain                     boundary (ptr)
struct aiDIE_BASE;    // ai08 — death/dying module               boundary (ptr)
struct aiWEAPON_BASE; // ai08 — weapon module                    boundary (ptr)
struct aiHO_SYS_BASE; // ai08 — hideout system module            boundary (ptr)
struct mdlITRC_BASE;  // mdl — interactive transform ref component boundary (ptr)

// DB-verified layout (types_members aiBODY_IFACE): __vftable@0, pBrain@4, die@8, wpn@0x10,
// hoSys@0x18. The full vtable (37 slots) is still the next frontier.
struct aiBODY_IFACE {
    // Filter selector for GetCurrentItrcHO (DB nested enum aiBODY_IFACE::aiBODY_GET_ITRCHO). The DB
    // carries no enumerator values; used only as a boolean gate (non-zero => skip the itrc IsValid
    // check). Enumerator values are INFERRED (unverified) — the two observed modes only.
    enum aiBODY_GET_ITRCHO { AIBODY_GET_ITRCHO_VALID_ONLY = 0, AIBODY_GET_ITRCHO_ANY = 1 };

    // DB-verified nested type (types_members aiBODY_IFACE::aiPTS_NEAR_BODY_ADDITIONAL_INFO), size 16:
    // the attacker/target entity + FSM pair filled by GetHOPointsNearBody.
    struct aiPTS_NEAR_BODY_ADDITIONAL_INFO {
        entENTITY *entTarget;   // 0x00
        entENTITY *entAttacker; // 0x04
        propFSM   *fsmTarget;   // 0x08
        propFSM   *fsmAttacker; // 0x0C
    };

    void    *__vftable; // 0x00 aiBODY_IFACE_vtbl* (slots 0..36)
    aiBRAIN *pBrain;    // 0x04 owning brain
    dsSHARED_PTR<aiDIE_BASE,    0, Deleter<aiDIE_BASE> >    die;   // 0x08 death/dying module
    dsSHARED_PTR<aiWEAPON_BASE, 0, Deleter<aiWEAPON_BASE> > wpn;   // 0x10 active weapon module
    dsSHARED_PTR<aiHO_SYS_BASE, 0, Deleter<aiHO_SYS_BASE> > hoSys; // 0x18 hideout system module

    // 0x832AB898 (?ProcessFrame@aiBODY_IFACE@@UAAXM@Z) — virtual: per-frame body update — tick the
    // weapon module, and (unless the brain is dead) tick the hideout system.
    // REVERSED: src/ws/ai/aiBODY_IFACE__ProcessFrame.cpp.
    void ProcessFrame(float dt);

    // Virtual — write the body's current upper-body facing direction into `out`.
    // Used by aiBRAIN::UpdateBodyCS.  boundary (body external to this batch).
    void GetUpperBodyDir(m3dV *out);

    // 0x832AB880 (?PreProcessInit@aiBODY_IFACE@@UAAXXZ) — virtual: body-side pre-process init.
    // Used by aiBRAIN::PreProcessINIT.  boundary (body external to this batch).
    void PreProcessInit();

    // Virtual (vtbl+0x30) — compute the body's face direction/position into `dir`/`pos`; returns
    // true when the body supplied them. Used by aiBRAIN::GetFaceParam. boundary (body external).
    bool GetFaceParam(m3dV *dir, m3dV *pos);

    // Virtual — per-level body init. Used by aiBRAIN::ProcessINIT_LEVEL. boundary.
    void ProcessInitLevel();

    // ?DbgGetMatrShapeCdt@aiBODY_IFACE@@UBAXAAVm3dMATR@@@Z @ 0x83197A88 — virtual, const: write the
    // body's collision-shape transform into `out` (debug LOF sphere placement). boundary.
    void DbgGetMatrShapeCdt(m3dMATR &out) const;

    // Virtual, const — the body's collision-shape (CDT) radius for the debug sphere. boundary.
    float DbgGetRadCdt() const;

    // Virtual — true when the body's melee attack is currently aligned on its target. Used by
    // aiWEAPON::IsAimed (aimBY_FSM path). boundary — body external to this batch.
    bool IsAimedForMelee();

    // vtbl slot 14 (byte offset 0x38) — apply any pending SSL "set parameter" request to the body;
    // returns true when the body itself consumed it (so the mind is not asked). Dispatched by
    // DB-verified slot index. Used by aiBRAIN::implSetPar.
    bool SetPar() {
        typedef bool (*Fn)(aiBODY_IFACE *);
        Fn fn = reinterpret_cast<Fn *>(__vftable)[14];
        return fn(this);
    }

    // vtbl slot 34 — the body's property FSM.
    propFSM *GetBodyProp() {
        typedef propFSM *(*Fn)(aiBODY_IFACE *);
        Fn fn = reinterpret_cast<Fn *>(__vftable)[34];
        return fn(this);
    }

    // vtbl slot 35 — the body's cached property bit-array (ctrlPROP_LIST). Unified return type: the
    // consumers that need the raw 32-bit word array reinterpret_cast the result (ctrlPROP_LIST is a
    // 12-cell dsFIXED_BIT_ARRAY, so its address is exactly that word array). Formerly diverged with
    // the aiWATCHER_BRAIN boundary subset (const unsigned int* vs const ctrlPROP_LIST*); collapsed.
    // DB: ?GetRefPropCache@aiBODY_IFACE@@UBAABUctrlPROP_LIST@@XZ — virtual, const, returns a
    // const ctrlPROP_LIST& (ABI-equivalent to the pointer form the consumers use).
    const ctrlPROP_LIST *GetRefPropCache() const {
        typedef const ctrlPROP_LIST *(*Fn)(const aiBODY_IFACE *);
        Fn fn = reinterpret_cast<Fn *>(__vftable)[35];
        return fn(this);
    }

    // Virtual — current hideout ITRC (by value). Used by aiWATCHER_BRAIN::GetHOMatrSnap. boundary.
    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > GetCurrentItrcHO();
    // Virtual (vtbl+0x70) — fetch the current ITRC into `out`; true if one exists. Used by
    // aiWATCHER_BRAIN::GetHideoutDir. boundary.
    bool GetCurrentItrc(dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > *out);

    // --- smart-object interaction virtuals reached by aiBRAIN::implUseSmart* / implAttachBodyToSmtObj.
    //     Bodies external to this batch (the next frontier).  boundary ---
    // Virtual — can this body enter smart object `smtObj` at entry point `entryInfo`?
    bool CanUseSmart(dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > *smtObj,
                     const dsPAIR<dsSTRID, dsSTRID> *entryInfo);
    // Virtual — attach this body to smart object `smtObj` at `entryInfo` (flag: attach mode); true on success.
    bool AttachToSmart(dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > *smtObj,
                       dsPAIR<dsSTRID, dsSTRID> *entryInfo, int flag);
    // Virtual — fire the body's FSM callback for event id `ev`.
    void ActivateBodyCB(const dsSTRID *ev, void *param);

    // vtbl slot 23 (byte offset 0x5C) — send FSM event `ev` (name id) with parameter block `param`;
    // returns whether the body's FSM handled it. Dispatched by DB-verified slot index. Used by
    // aiBRAIN::implSetInterest.
    bool SendFsmEventByName(const dsSTRID *ev, void *param) {
        typedef bool (*Fn)(aiBODY_IFACE *, const dsSTRID *, void *);
        Fn fn = reinterpret_cast<Fn *>(__vftable)[23];
        return fn(this, ev, param);
    }
    // vtbl slot 24 (byte offset 0x60) — send parameterless FSM event `ev`; returns whether handled.
    // Used by aiBRAIN::implEndInterest.
    bool SendFsmEventByName_2(const dsSTRID *ev) {
        typedef bool (*Fn)(aiBODY_IFACE *, const dsSTRID *);
        Fn fn = reinterpret_cast<Fn *>(__vftable)[24];
        return fn(this, ev);
    }

    // vtbl slot 27 (byte offset 0x6C) — notify the body of an alert at world position `pos`.
    // Dispatched by DB-verified slot index (m3dV passed by value in the ABI). Used by
    // aiBRAIN::OnAlert.  boundary — body external to this batch.
    void NoticeAlert(const m3dV *pos) {
        typedef void (*Fn)(aiBODY_IFACE *, const m3dV *);
        Fn fn = reinterpret_cast<Fn *>(__vftable)[27];
        fn(this, pos);
    }

    // 0x832ABF20 (?FillCache@aiBODY_IFACE@@UAAXXZ) — virtual: refresh per-frame cached body state;
    // the base implementation just forwards to the weapon module's var cache (when alive+armed).
    // REVERSED: src/ws/ai/aiBODY_IFACE__FillCache.cpp.
    void FillCache();

    // (?GetBodyInfo@aiBODY_IFACE@@UAAPAVmdlINFO_BASE@@XZ) — virtual: the body's model info record
    // (null when none). Used by aiBRAIN::GetCdtParams.  boundary — body external to this batch.
    struct mdlINFO_BASE *GetBodyInfo();

    // Virtual — teleport the body to world position `pos`. Used by aiBRAIN::Transport(const m3dV*).
    // boundary — body external to this batch.
    void Transport(const m3dV *pos);
    // Virtual — advance the body's FSM one step. Used by aiBRAIN::ProcessFRAME.  boundary.
    void UpdateFsm();
    // Virtual — drop the per-frame cached body state (paired with FillCache). Used by
    // aiBRAIN::ProcessFRAME.  boundary — body external to this batch.
    void ClearCache();
};
