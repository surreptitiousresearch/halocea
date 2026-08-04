#pragma once
#include "../msg/msgADDR.h"
#include "../msg/msgRES.h"
#include "../ds/WEAK_PTR_BASE.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsTYPE_ID.h"
#include "../ap/apSTATE_T.h"
#include "../ssl/sslOBJ_REF.h"
#include "../dsc/dscDESC.h"
#include "../prop/propCONTAINER.h"
#include "../ps/psSECTION.h"
#include "../sml/STATE.h"
#include "iaIACTOR_SOUND.h"
#include "iaSND_MUTE_TYPE.h"
#include "iaSND_PLAY_DATA.h"
#include "../ds/dsSTRID.h"
#include "../ds/LIST.h"
// ws-engine ia (interactive actor) subsystem: base class for engine-message-addressable game
// actors. The leading base (offset 0, size 32) is msgADDR — modeled as public inheritance per
// convention so derived classes (entENTITY) can access inherited msgADDR fields (e.g. stateAddr)
// directly.
// DB-verified layout (types_members iaIACTOR):
//   msgADDR@0 (32B, base), ds::WEAK_PTR_BASE<iaIACTOR>@32 (4B), stateSml@36 (sml::STATE, 12B),
//   snd@48 (iaIACTOR_SOUND, 36B), spDesc@84 (dsSMART_PTR<dscDESC,...>, 4B), sslObject@88
//   (sslOBJ_REF, 4B), stateSslEv@92 (apSTATE_T<unsigned long>, 4B), propContainer@96
//   (propCONTAINER, 80B), nmbLocks@176 (int, 4B), nameIActive@180 (dsTSTRING<char>, 4B),
//   typeId@184 (dsTYPE_ID, 16B) — total size 200 (0xC8).

struct iaIACTOR : msgADDR {
    ds::WEAK_PTR_BASE<iaIACTOR>                                   weakSelf;      // 0x20
    sml::STATE                                                    stateSml;      // 0x24 active sml state bitmask, owned by this actor
    iaIACTOR_SOUND                                                snd;           // 0x30 per-actor sound state — boundary (see iaIACTOR_SOUND.h)
    dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc;        // 0x54 owned descriptor (dynamic type is an iaDESC or deeper)
    sslOBJ_REF                                                    sslObject;     // 0x58
    apSTATE_T<unsigned long>                                      stateSslEv;    // 0x5C
    propCONTAINER                                                 propContainer; // 0x60
    int                                                           nmbLocks;      // 0xB0
    dsTSTRING<char>                                               nameIActive;   // 0xB4
    dsTYPE_ID                                                     typeId;        // 0xB8

    // 0x826E76A0 (?FindMarkedSound@iaIACTOR@@QAA?AVITERATOR@?$LIST@ViaSND_PLAY_DATA@@@ds@@VdsSTRID@@V234@@Z)
    // — search this actor's playing-sound list from `startFrom` for the next sound tagged with RPC
    // marker `mark`; returns an iterator (end iterator when none). boundary.
    ds::LIST<iaSND_PLAY_DATA>::ITERATOR FindMarkedSound(dsSTRID mark,
                                                        ds::LIST<iaSND_PLAY_DATA>::ITERATOR startFrom);

    // Virtual — per-level actor init. Called (as the base impl) by aiBRAIN::ProcessINIT_LEVEL.
    // Body external to this batch. boundary.
    int ProcessINIT_LEVEL();

    // Virtual — one-time actor init (subsystem registration). Called (as base impl) by
    // aiPLANNER::ProcessINIT. Body external to this batch. boundary.
    int ProcessINIT(void *pInfo);

    // Boundary default constructor (real body at a DB address; not reversed here). Declared so the
    // implicit default ctor is not deleted by a member with a deleted/inaccessible default ctor,
    // letting derived classes (e.g. HALO_MNG) base-construct iaIACTOR. Layout-neutral.
    iaIACTOR();

    // 0x826D8010 — adopt `spDescArg`'s referent as this actor's owned descriptor, releasing the
    // previous one; then recompute `typeId` from the new descriptor's runtime type (or fall back
    // to iaDESC::TYPE_ID when the new descriptor is empty). QAAX (public, non-const).
    void SetDesc(dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDescArg);

    // 0x826D8450 — clone this actor's descriptor to a private copy (via UnshareDescByInst) before
    // applying `section`'s config, when either `section` (and its parent hierarchy) has keys to
    // apply, or the owning propCONTAINER_DESC says an instance-level unshare is needed anyway.
    // QAAX (public, non-const).
    void UnshareDescIfNeeded(psSECTION section, const dsTSTRING<char> &hintErr);

    // 0x826D8450's callee — protected virtual: perform the actual unshare-and-reparse for a
    // by-value `section`/`hintErr` pair. Not reversed in this batch (dispatched through iaIACTOR's
    // own vtable, slot 0xD0 / 52). MAAX (protected, non-const), virtual. boundary.
    virtual void UnshareDescByInst(psSECTION section, const dsTSTRING<char> &hintErr);

    // Frame/message-dispatch virtuals (added this session, DB-verified via mangled name for the
    // HALO_MNG override chain — see src/hcex/HALO_MNG__*.cpp). Bodies not decompiled here; boundary.
    virtual void ProcessTERM_LEVEL();                                          // 0x826D5958 protected virtual
    virtual msgRES ProcessMsg(int msg, void *pInfo, msgADDR *pSend);           // 0x826D5B00 protected virtual
    virtual unsigned int GetSuspendMask();                                     // 0x826D5C68 protected virtual
    virtual void OnChangeMode(unsigned int oldMode, unsigned int newMode);      // 0x826D5C88 protected virtual
    virtual void ProcessFRAME();                                               // 0x826D67E8 protected virtual

    // 0x823D05C8 (?TypeID@iaIACTOR@@UBA?AVdsTYPE_ID@@XZ) — public virtual RTTI accessor: returns
    // this actor's dynamic dsTYPE_ID by value. boundary (body external to this batch).
    virtual dsTYPE_ID TypeID() const;

    // Public virtual: set this actor's instance name. DB-verified slot (types_members
    // HALO_MNG_vtbl::SetName @0x34, `void (*)(this, const dsTSTRING<char>*)`); resolves the
    // former haloInit.cpp raw offset-0x34 vtable dispatch CAVEAT. boundary.
    virtual void SetName(const dsTSTRING<char> *name);

    // 0x823D4968 (?GetName@iaIACTOR@@UBA?AV?$dsTSTRING@D@@XZ) — public virtual: this actor's instance
    // name string, returned by value. boundary (body external to this batch).
    virtual dsTSTRING<char> GetName() const;

    // Public, non-virtual: elapsed time since this actor's last frame tick (used by
    // HALO_MNG::ProcessFRAME to drive hcex_update_anim). Not reversed here; boundary.
    float GetElapsedTime() const;                                              // 0x826D6050

    // 0x823DB164 (GetProperty<propHALO_OBJ>) / 0x823DB06C (GetProperty<propINST_CONSTRUCTOR>) /
    // 0x823DFC6C (GetProperty<propOBJ_MODIFIER_LIST>) — locate a property of type P owned by this
    // actor's propContainer: if propContainer.typeId itself IsDerivedFrom P's DESC::TYPE_ID,
    // return propContainer reinterpreted as P*. Otherwise, only when propContainer.typeId
    // IsDerivedFrom propCONTAINER_DESC::TYPE_ID (i.e. this actor really owns a property
    // collection), linear-scan propContainer.properties then propContainer.components, calling
    // each entry's propBASE::GetProperty<P>() (boundary — see propBASE.h) and returning the first
    // non-null hit; nullptr if none match anywhere. const-correctness matches the DB (QBA mangle
    // absent — these are non-const QAA calls in the disassembly). Reversed per-instantiation.
    template<class P> P *GetProperty();

    // 0x826E9D20 (?PlaySoundSynced@iaIACTOR@@QAAPAViaSND_PLAY_DATA@@VdsSTRID@@H@Z) — start sound
    // `sndID` on this actor, synchronized (mode `sync`); returns the resulting play record (null on
    // failure). boundary. (iaSND_PLAY_DATA — see iaSND_PLAY_DATA.h.)
    struct iaSND_PLAY_DATA *PlaySoundSynced(dsSTRID sndID, int sync);
    // 0x826EABA8 (?MuteSound@iaIACTOR@@QAAXVdsSTRID@@W4iaSND_MUTE_TYPE@@@Z) — silence sound `sndID`
    // on this actor with the given mute policy. boundary.
    void MuteSound(dsSTRID sndID, iaSND_MUTE_TYPE muteType);
};
