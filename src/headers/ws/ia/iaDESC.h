#pragma once
#include "../ssl/sslDESC.h"
#include "../DEFAULT_CTOR.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsTYPE_ID.h"
#include "../ap/apSTATE_T.h"
#include "../ps/psSECTION.h"
#include "../prop/propCONTAINER_DESC.h"
#include "../sml/smlSTATE_ID.h"
#include "../sml/STATE_DESC.h"
#include "iaSYNC_MODE.h"
// ws-engine ia (interactive actor) subsystem: base descriptor class for entDESC (and other
// ia-derived descriptors) — the shared, parsed config template an iaIACTOR's spDesc points at.
// Carries the SSL-callback function-id cache (resolved once at parse time so per-instance script
// dispatch is a table lookup, not a name lookup), the owned property-container descriptor, and
// the actor's default lock/priority/sync config.
// DB-verified layout (types_members iaDESC): sslDESC@0 (20B, base), isSslClassInited@20,
// sslEventId_OnTerm@24, sslFuncId_Lock..sslFuncId_DoSuspend@28-96 (18 ints), __smlState_START@100,
// propContainerDesc@104, psCustomSection@108, state@112, priority@116, curLocks@120, maxLocks@124,
// minLocks@128, smlProg@132, smlDesc@136 (sml::STATE_DESC, 20B), spSndData@156, syncMode@160
// (iaSYNC_MODE, 4B) — total size 164 (0xA4).

struct iaIACTOR; // ws-engine actor — opaque boundary (only referenced by pointer here)
struct smlPROGRAMM; // ws-engine sml bytecode program — boundary, only pointed to via dsSMART_PTR
struct iaSND_DATA;  // ws-engine per-descriptor sound data — boundary, only pointed to via dsSMART_PTR

// DB-verified (types_members iaDESC_vtbl, 40 bytes / 10 slots). Extends sslDESC_vtbl (9 slots,
// 36 bytes) with one new virtual, TypeID, at slot 9 (offset 0x24). dscDESC/sslDESC/iaDESC model
// their vtable as a plain function-pointer-table member (not C++ `virtual`) so a base pointer's
// inherited `__vftable` field keeps its narrower static type (dscDESC_vtbl*) even when the real
// object is an iaDESC — callers that need slot 9 must reinterpret it as `const iaDESC_vtbl *`
// (see iaIACTOR::SetDesc, which does exactly this to call TypeID).
typedef struct iaDESC_vtbl {
    const dscTYPE_INFO *(*GetCurTypeInfo)(iaDESC *self);                                          // 0x00
    void                (*dtr_iaDESC)(iaDESC *self, int deleteFlag);                                              // 0x04 (deleting dtr; takes a free flag) — deleting dtor: vftable+0x04 holds ??_EiaDESC@@UAAPAXI@Z
    void                (*NotifyTermBrand)(iaDESC *self);                                         // 0x08
    void                (*ParsePS)(iaDESC *self, psSECTION section, const dsTSTRING<char> &hintErr);       // 0x0C
    void                (*PostProcessPS)(iaDESC *self, psSECTION section, const dsTSTRING<char> &hintErr); // 0x10
    sslCLASS_REF        *(*GetSslClass)(iaDESC *self, sslCLASS_REF *result);                       // 0x14
    int                 (*InitSslClass)(iaDESC *self, const dsTSTRING<char> &name);                 // 0x18
    void                (*UnshareSslClass)(iaDESC *self, bool, bool);                              // 0x1C
    bool                (*IsNeedToUnshareSslClass)(iaDESC *self, bool);                             // 0x20
    dsTYPE_ID           *(*TypeID)(iaDESC *self, dsTYPE_ID *result);                                // 0x24
} iaDESC_vtbl;

struct iaDESC : sslDESC {
    // Numeric sml state id for this actor class's START (spawned/active) state (__smlState_START
    // @ 0x64). Read by entENTITY show/hide to gate the START bit. boundary.
    int GetState_START() const;

    DEFAULT_CTOR<bool>                                                       isSslClassInited;    // 0x14
    unsigned char                                                            _pad15[3];           // 0x15
    int                                                                      sslEventId_OnTerm;   // 0x18
    int                                                                      sslFuncId_Lock;              // 0x1C
    int                                                                      sslFuncId_Unlock;            // 0x20
    int                                                                      sslFuncId_SetState;          // 0x24
    int                                                                      sslFuncId_IsState;           // 0x28
    int                                                                      sslFuncId_Name;              // 0x2C
    int                                                                      sslFuncId_IsName;            // 0x30
    int                                                                      sslFuncId_IsLockedNow;       // 0x34
    int                                                                      sslFuncId_IsKindOf;          // 0x38
    int                                                                      sslFuncId_PlaySound;         // 0x3C
    int                                                                      sslFuncId_PlaySoundVolume;   // 0x40
    int                                                                      sslFuncId_MuteSound;         // 0x44
    int                                                                      sslFuncId_SetSndParams;      // 0x48
    int                                                                      sslFuncId_ShowMsg;           // 0x4C
    int                                                                      sslFuncId_Terminate;         // 0x50
    int                                                                      sslFuncId_SoundTime;         // 0x54
    int                                                                      sslFuncId_mpLock_;           // 0x58
    int                                                                      sslFuncId_mpUnlock_;         // 0x5C
    int                                                                      sslFuncId_DoSuspend;         // 0x60
    smlSTATE_ID                                                             __smlState_START;    // 0x64
    dsSMART_PTR<propCONTAINER_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > propContainerDesc;   // 0x68
    psSECTION                                                               psCustomSection;      // 0x6C
    apSTATE_T<unsigned long>                                                state;                // 0x70
    int                                                                      priority;             // 0x74
    int                                                                      curLocks;             // 0x78
    int                                                                      maxLocks;             // 0x7C
    int                                                                      minLocks;             // 0x80
    dsSMART_PTR<smlPROGRAMM, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >        smlProg;              // 0x84
    sml::STATE_DESC                                                         smlDesc;              // 0x88 per-owner registered sml state ids
    dsSMART_PTR<iaSND_DATA, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >         spSndData;            // 0x9C
    iaSYNC_MODE                                                             syncMode;             // 0xA0

    // Process-wide default type-id for a plain iaDESC-described actor (used by iaIACTOR::SetDesc
    // as the fallback when the actor has no descriptor set). Defined out of line. boundary.
    static dsTYPE_ID TYPE_ID;

    // 0x826AEE58 — resolve `nameState` to its numeric sml state id for actors described by this
    // iaDESC: forwards to sml::STATE_DESC::Register on the embedded smlDesc table. QAAH (public,
    // non-const), returns int (the nameIdx position within smlDesc, per STATE_DESC::Register).
    int RegisterSmlState(const dsTSTRING<char> &nameState);

    // Base descriptor parse (virtual slot 0x0C ParsePS body). Derived ParsePS overrides
    // (aiPLANNER_DESC/aiCACHER_DESC) chain up to this to read the shared iaDESC config from the
    // ps section. boundary — body external to this batch.
    void ParsePS(psSECTION section, const dsTSTRING<char> &hintErr);

    // 0x826AF188 (?SetPriority@iaDESC@@IAAXH@Z) — protected: store `priority` into the descriptor.
    void SetPriority(int priority); // boundary

    // 0x823E52F8 — copy-assign (added this session, DB-verified via mangled name for the
    // HALO_MNG_DESC override chain — see src/hcex/HALO_MNG_DESC__operatorAssign.cpp). Body not
    // reversed here; boundary.
    iaDESC &operator=(const iaDESC &that);
};
