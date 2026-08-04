#pragma once
#include "aiBEST_IFACE.h"
#include "b2bPAR_STORAGE.h"
#include "bhvBEHAV.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
#include "../ds/WEAK_PTR.h"
// ws-engine ai08: the concrete best-behaviour selection module a brain's mind drives (derives from
// aiBEST_IFACE : aiBEST_BASE). DB-verified layout (types_members aiBEST) — size 224:
//   aiBEST_IFACE base@0 (8), arrExecutable@8, arrChangeable@0x34, arrToDel@0x60, state@0xB4,
//   bhvContextSNum@0xB8, parcelStorage@0xBC, delayedNoticeGrenade@0xD4, dbgLine@0xD8, errorLine@0xDC.

struct entENTITY; // ent — entity (fwd; delayedNoticeGrenade referent)
struct aiBRAIN;   // ai — owning brain (fwd; constructor arg)
struct bhpPARAMS; // bhp — base behaviour params (fwd; SetBehaviorByParams arg)

// ?BEST_ST — best-behaviour module state bitflags (DB types_enum_values BEST_ST).
enum BEST_ST {
    BEST_ST_SETBHV_DISABLED     = 1,
    BEST_ST_BACKGROUND_DISABLED = 2,
    BEST_ST_COMMCHG_IN_PROCESS  = 4,
};

struct aiBEST : aiBEST_IFACE {
    // aiBEST::<unnamed_type_delayedNoticeGrenade> — a single weak-ptr to a pending-notice grenade.
    struct DELAYED_NOTICE_GRENADE {
        ds::WEAK_PTR<entENTITY> grenade; // 0x00
    };

    dsCONST_ARRAY<bhvBEHAV *,10>  arrExecutable;        // 0x08 flattened execution stack
    dsCONST_ARRAY<bhvBEHAV *,10>  arrChangeable;        // 0x34 live (changeable) behaviour stack
    dsCONST_ARRAY<bhvBEHAV *,20>  arrToDel;             // 0x60 popped behaviours pending trash
    dsFLAGS<BEST_ST,int>          state;                // 0xB4
    unsigned int                  bhvContextSNum;       // 0xB8
    b2bPAR_STORAGE                parcelStorage;        // 0xBC
    DELAYED_NOTICE_GRENADE        delayedNoticeGrenade; // 0xD4
    dsTSTRING<char>               dbgLine;              // 0xD8
    dsTSTRING<char>               errorLine;            // 0xDC

    // ??0aiBEST@@QAA@PAVaiBRAIN@@@Z — construct concrete best module for a brain. 0x83289E10.
    aiBEST(aiBRAIN *pBrain);

    // ---- virtual stack queries (bodies external) — boundaries ----
    virtual bhvBEHAV *GetTopBehavior();                       // vtbl+0xB4
    virtual bhvBEHAV *GetParentBehavior(const bhvBEHAV *child); // vtbl+0xC0

    // 0x832B9590 (?CreateScriptedBehavior@aiBEST@@UAAPAVbhvBEHAV@@PAD@Z) — virtual: instantiate the
    // scripted behaviour named `namePS`. boundary (body external to this batch).
    virtual bhvBEHAV *CreateScriptedBehavior(char *namePS);

    // ---- reversed in this batch (aiSND/aiBEST behaviour-stack drain) ----
    // 0x83298BE0 (?SetBehavior@aiBEST@@UAAHPAD@Z) — virtual: replace the whole stack with scripted
    // behaviour `namePS`. REVERSED: aiBEST__SetBehavior.cpp.
    virtual int SetBehavior(char *namePS);
    // 0x83298EA0 (?PushBehavior@aiBEST@@UAAHPAD@Z) — virtual: push scripted behaviour `namePS`.
    // REVERSED: aiBEST__PushBehavior_scripted.cpp.
    virtual int PushBehavior(char *namePS);
    // 0x83298CC0 (?PushBehaviorByParams@aiBEST@@UAA_NPAVbhpPARAMS@@HPBD@Z) — virtual override: build,
    // init and push a behaviour from `pars`. REVERSED: aiBEST__PushBehaviorByParams.cpp.
    bool PushBehaviorByParams(bhpPARAMS *pars, int isMajor, const char *SLID);
    // 0x832990B0 (?SetupInitialBehavior@aiBEST@@UAAXXZ) — virtual override: seed the initial behaviour
    // after per-level setup. REVERSED: aiBEST__SetupInitialBehavior.cpp.
    void SetupInitialBehavior();
    // 0x83298AF0 (?PushBehavior@aiBEST@@AAAHPAVbhvBEHAV@@@Z) — private: push an already-built behaviour.
    // REVERSED: aiBEST__PushBehavior.cpp.
    int PushBehavior(bhvBEHAV *toPush);

    // ---- reversed in this batch ----
    void PopBehavior(bhvBEHAV *pBehav);                         // 0x83296DF0
    void SendTopNotifications(bhvBEHAV *pBehav);                // 0x83297010
    void TrashPopped();                                        // 0x83297108
    void FillExecStack();                                      // 0x83297270
    void ActivateNewBehaviors();                               // 0x832973F0
    bool AddBhvFuncCall(const dsTSTRING<char> &bhvPrefix,
                        const dsTSTRING<char> &nameFunc);       // 0x832974E8 (virtual override)
    void CommitChanges();                                      // 0x832975D0
    void PreformNoticeGrenade();                               // 0x83297690
    void CommonPFAfterBody(void (bhvBEHAV::*proc)());          // 0x83297738
    void CommonPFBeforeBody(void (bhvBEHAV::*proc)());         // 0x832977E8
    void UpdateTimers(float dt);                               // 0x83296890 (virtual override)

    // ---- reversed in this batch (aiBEST stack lifecycle) ----
    void PerformDelayedCalls();                                // 0x832978D0
    void ProcessFrameAfterBody();                              // 0x83297960 (virtual override)
    void ProcessFrameBeforeBody();                             // 0x83297A10 (virtual override)
    void EndBodyResponsibleMajor();                            // 0x83297EE8
    void ForcedClear();                                        // 0x832984B0 (virtual override)
    int  AddToChangeable(bhvBEHAV *pBehav);                    // 0x83298550
    int  ReplaceAllStack(bhvBEHAV *pBehav);                    // 0x83298678
    void AddBehaviorOnEmptyStack();                            // 0x83298740 (virtual)
    bool SetBehaviorByParams(bhpPARAMS *pars, const char *SLID); // 0x83298930 (virtual override)

    // ---- callee boundaries (bodies external to this batch) ----
    int       FindMajorResponsibleForBody();
    void      BeginBodyResponsibleMajor(int majorIdx);
    void      AddToDel(bhvBEHAV **pBehav);
    void      IssueTermCalls();
    void      EnsureNonEmpty();
    bhvBEHAV *FindNonActivated();
    void      EraseAll(BHV_ST reason);
    bool      IsEmpty();                                       // true when the changeable stack is empty
    bhvBEHAV *WrappedNewBehavior(int typeBehav);              // create a behaviour object of the given aiBID
    void      InactivateMajor();                               // 0x8330C728 deactivate the current major
};
