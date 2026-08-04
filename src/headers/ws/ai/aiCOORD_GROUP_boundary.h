#pragma once
// Boundary declaration of aiCOORD_GROUP — the coordinated squad/group actor (ai08).
// The full data layout is DB-known (types_members aiCOORD_GROUP, size 0x1F8, 29 members:
// groupName/groupType/members/target/wpSet/... ) and is the next frontier; it is intentionally
// NOT reproduced here because the ssl callback wrappers in src/ws/ai/aiCOORD_GROUP__cb*.cpp touch
// no data members directly — they only dispatch the virtuals declared below. Only method decls
// live here (per "No duplicate type definitions": add method decls, never a competing full body).

struct dsDATA;      // ds type-erased value        (full definition in ds/dsDATA.h)
// sslOBJ_REF is passed BY VALUE to the cb* callbacks below, so consumers need its full
// definition (C2027) — include it here rather than forward-declaring.
#include "../ssl/sslOBJ_REF.h"
struct iaIACTOR;    // interactive actor           (full definition in ia/iaIACTOR.h)
struct aiBRAIN;     // ai08 per-unit AI brain      (full definition in ai/aiBRAIN.h)
#include "../ds/dsSTRID.h"                      // dsSTRID — needed complete for the static GRP_* members below
typedef struct sslCLASS_REF sslCLASS_REF;       // ssl class ref (full definition in ssl/sslCLASS_REF.h)
template<class T> struct dsTSTRING; // ds string   (full definition in ds/dsTSTRING.h)

struct navSYS;      // nav subsystem — a single nav-system   boundary (ptr)

struct aiCOORD_GROUP {
    // The group's owning nav system (null when it has none). Dispatched by aiSQUAD::GetWPSet;
    // declared non-virtual to avoid claiming an unresolved vtable slot. Body: next frontier.
    navSYS *GetNavSystem();

    // --- underlying virtuals (2-level descent boundary; bodies are the next frontier) ---
    virtual bool  Is3DNavSys(iaIACTOR *host);                              // 0x832BC6A8
    virtual float GetTargetMovementDeltaDist();                           // 0x832BBBD0
    virtual float GetTargetMovementDeltaDistViaNavSys();                  // 0x832C1AE8
    virtual float GetDistToTargetDirect(iaIACTOR *host);                  // 0x832BC838
    virtual float GetDistToTargetViaNavSys(iaIACTOR *host);               // 0x832C1C70
    virtual bool  HaveWPSet();                                            // 0x832BC190
    virtual int   GetWPSetCount();                                        // 0x832BC1A0
    virtual bool  EnsureHaveWPSet();                                      // 0x832C8F20
    virtual bool  IsTargetWPInWPSet(iaIACTOR *host);                      // 0x832C1F20
    virtual bool  GetWPSetFromNavSys();                                   // 0x832C7010
    virtual bool  GetWPSetFromTargetAssocDistFn(int a, bool b, float c);  // 0x832C7530
    virtual bool  GetWPSetFromTargetDistFn(int a);                        // 0x832C7938

    // --- WPSet HO / chaser + filter virtuals (this batch; bodies are the next frontier) ---
    virtual bool  GetWPSetHOFromTargetDistFn(int a);                      // dispatched by cbGetWPSetHOFromTargetDistFn
    virtual bool  GetWPSetFromChaser(iaIACTOR *chaser);                   // dispatched by cbGetWPSetFromChaser
    virtual bool  FilterWPSetByTargetAssocDistFn(int a, bool b, float c); // cbFilterWPSetByTargetAssocDistFn
    virtual bool  FilterWPSetByTargetDistFn(int a);                       // cbFilterWPSetByTargetDistFn
    virtual bool  FilterWPSetByTargetAngleFn(int argc, dsDATA *argv);     // cbFilterWPSetByTargetAngleFn (raw argc/argv)
    virtual bool  FilterWPSetByMaxDistToTarget(float dist);              // cbFilterWPSetByMaxDistToTarget
    virtual bool  FilterWPSetByDistToEnemy(iaIACTOR *host, int a);       // cbFilterWPSetByDistToEnemy
    virtual bool  FilterWPSetByPeerPenalty(iaIACTOR *host);              // cbFilterWPSetByPeerPenalty
    virtual bool  FilterWPSetByHO(bool a);                              // cbFilterWPSetByHO
    virtual bool  FilterWPSetByHOvsEnemy(iaIACTOR *host, float a);      // cbFilterWPSetByHOvsEnemy
    virtual bool  FilterWPSetByHOvsTargetHODir(int argc, dsDATA *argv); // cbFilterWPSetByHOvsTargetHODir (raw argc/argv)
    virtual bool  FilterWPSetByAngleVsTarget(iaIACTOR *host, float a);  // cbFilterWPSetByAngleVsTarget

    // --- membership / target virtuals (batch: 0x8322F920..0x832300F8 ssl wrappers) ---
    virtual void      RemoveMember(iaIACTOR *member);   // ?RemoveMember@aiCOORD_GROUP@@UAAXPAViaIACTOR@@@Z
    virtual bool      LeadGroup(iaIACTOR *member);      // ?LeadGroup@aiCOORD_GROUP@@UAA_NPAViaIACTOR@@@Z
    virtual bool      HaveMembers();                    // ?HaveMembers@aiCOORD_GROUP@@UAA_NXZ
    virtual int       GetMembersCount();                // ?GetMembersCount@aiCOORD_GROUP@@UAAHXZ
    virtual iaIACTOR *GetMember(int index);             // ?GetMember@aiCOORD_GROUP@@UAAPAViaIACTOR@@H@Z
    virtual iaIACTOR *GetMemberSorted(int index);       // ?GetMemberSorted@aiCOORD_GROUP@@UAAPAViaIACTOR@@H@Z
    virtual bool      IsContain(iaIACTOR *member);      // ?IsContain@aiCOORD_GROUP@@UAA_NPAViaIACTOR@@@Z
    virtual bool      IsGroupLeader(iaIACTOR *member);  // ?IsGroupLeader@aiCOORD_GROUP@@UAA_NPAViaIACTOR@@@Z
    virtual bool      IsTargetValid();                  // ?IsTargetValid@aiCOORD_GROUP@@UAA_NXZ
    virtual bool      IsTargetInHO();                   // ?IsTargetInHO@aiCOORD_GROUP@@UAA_NXZ
    virtual float     GetTimeTargetInHO();              // ?GetTimeTargetInHO@aiCOORD_GROUP@@UAAMXZ
    virtual bool      ChangeNSToTarget(int nsIndex);    // ?ChangeNSToTarget@aiCOORD_GROUP@@UAA_NH@Z

    // --- leader / target / name / membership virtuals (batch: 0x8322F218..0x8322F840 wrappers) ---
    virtual void      NoticeWounded(aiBRAIN *pBrain, float amount); // vtbl 0x14 dispatched by aiCOORDINATOR::NoticeWounded
    virtual const sslOBJ_REF     *GetSslObj();          // vtbl 0x08 ?GetSslObj — const sslOBJ_REF*
    virtual const dsTSTRING<char> *GetName();           // vtbl 0x48 ?GetName@aiCOORD_GROUP@@UAA...
    virtual const dsTSTRING<char> *GetType();           // vtbl 0x4C ?GetType@aiCOORD_GROUP@@UAA...
    virtual bool      AddMember(iaIACTOR *member);      // vtbl 0x50 ?AddMember@aiCOORD_GROUP@@UAA_NPAViaIACTOR@@@Z
    virtual iaIACTOR *GetLeader();                      // vtbl 0x6C ?GetLeader@aiCOORD_GROUP@@UAAPAViaIACTOR@@XZ
    virtual iaIACTOR *GetTarget();                      // vtbl 0x80 ?GetTarget@aiCOORD_GROUP@@UAAPAViaIACTOR@@XZ

    // --- WPSet filter / curve / target virtuals (2-level descent boundary; bodies next frontier) ---
    virtual bool FilterWPSetByDistToEnemyLessDist(iaIACTOR *host, float dist);
    virtual bool FilterWPSetByDistToEnemySameDist(iaIACTOR *host, float dist);
    virtual bool FilterWPSetByDistFromBrain(iaIACTOR *host, float dist);
    virtual bool FilterWPSetByDistFromUsedWPs(iaIACTOR *host, float dist);
    virtual bool FilterWPSetByMinHeight(float minHeight);
    virtual bool FilterWPSetByTargetHOPlane();          // vtbl 0x14C; disasm-verified no-arg
    virtual void AddPossibleEnemiesFromTarget(iaIACTOR *host);
    virtual bool SetTarget(const dsTSTRING<char> *name);
    virtual bool AddNSToIgnoreList(const dsTSTRING<char> *name);
    virtual bool RemoveNSFromIgnoreList(const dsTSTRING<char> *name);
    virtual void SetDistFnCurve(const dsTSTRING<char> *name);
    virtual void SetAngleFnCurve(const dsTSTRING<char> *name);
    virtual bool IsEqualCurves(const dsTSTRING<char> *a, const dsTSTRING<char> *b, float tolerance);
    virtual bool IsValidFront(const dsTSTRING<char> *name);

    // --- front / excluder / squad-actor virtuals (a8_coord ssl batch; bodies next frontier) ---
    virtual bool      IsNeedFrontUpdate(const dsTSTRING<char> &front);        // 0x832BC040
    virtual bool      AddFrontToExcluders(const dsTSTRING<char> &front);      // 0x832C2C50
    virtual bool      RemoveFrontFromExcluders(const dsTSTRING<char> &front); // 0x832C09C8
    virtual bool      GetWPSetFromFront(const dsTSTRING<char> &front);        // 0x832C71E8
    virtual bool      FilterWPSetByFront(const dsTSTRING<char> &front);       // 0x832C2CE0
    virtual iaIACTOR *GetSquadActor(const dsTSTRING<char> &squadActorName);   // 0x832BC1A8

    // Broadcast target for aiCOORDINATOR::NoticeRemoveOtherGrpMember (per-group cross-ref cleanup).
    virtual void      NoticeRemoveOtherGrpMember(aiCOORD_GROUP *grp, aiBRAIN *pBrain);

    // ?NoticeChangeNavSystem@aiCOORD_GROUP@@... — a member brain changed its nav system; refresh the
    // group's shared nav-system state. Dispatched by aiGOAL::SetNavSystem. body: next frontier.
    virtual void      NoticeChangeNavSystem(aiBRAIN *pBrain);

    // --- ssl callback wrappers (this batch) — signature is the uniform ssl callback ABI:
    //     void (int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller). ---
    void cbIs3DNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbGetTargetMovementDeltaDist(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbGetTargetMovementDeltaDistViaNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbGetDistToTargetDirect(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbGetDistToTargetViaNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbHaveWPSet(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbGetWPSetCount(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbEnsureHaveWPSet(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbIsTargetWPInWPSet(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbGetWPSetFromNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbGetWPSetFromTargetAssocDistFn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbGetWPSetFromTargetDistFn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // membership / target ssl wrappers (this batch: 0x8322F920..0x832300F8)
    void cbRemoveMember(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);    // 0x8322F920
    void cbLeadGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x8322F9B8
    void cbHaveMembers(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);     // 0x8322FA98
    void cbGetMembersCount(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x8322FB28
    void cbGetMember(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x8322FBB8
    void cbGetMemberSorted(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x8322FCA0
    void cbIsContain(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x8322FD88
    void cbIsGroupLeader(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);   // 0x8322FE68
    void cbIsTargetValid(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);   // 0x8322FF48
    void cbIsTargetInHO(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);    // 0x8322FFD8
    void cbGetTimeTargetInHO(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83230068
    void cbChangeNSToTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);  // 0x832300F8

    // leader / target / name / member ssl wrappers (this batch: 0x8322F218..0x8322F840)
    void cbGetLeader(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);  // 0x8322F218
    void cbGetTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);  // 0x8322F2E0
    void cbGetName(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);    // 0x8322F670
    void cbGetType(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);    // 0x8322F758
    void cbAddMember(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);  // 0x8322F840

    // WPSet HO / chaser + filter ssl wrappers (this batch: 0x832309C8..0x832311A8)
    void cbGetWPSetHOFromTargetDistFn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x832309C8
    void cbGetWPSetFromChaser(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);         // 0x83230A60
    void cbFilterWPSetByTargetAssocDistFn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83230B40
    void cbFilterWPSetByTargetDistFn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);  // 0x83230C00
    void cbFilterWPSetByTargetAngleFn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83230C98
    void cbFilterWPSetByMaxDistToTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83230D28
    void cbFilterWPSetByDistToEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);   // 0x83230DC0
    void cbFilterWPSetByPeerPenalty(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);   // 0x83230EB0
    void cbFilterWPSetByHO(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);            // 0x83230F90
    void cbFilterWPSetByHOvsEnemy(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);     // 0x83231028
    void cbFilterWPSetByHOvsTargetHODir(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83231118
    void cbFilterWPSetByAngleVsTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x832311A8

    // --- methods dispatched by aiCOORDINATOR (this batch; bodies are the next frontier).
    //     Declared non-virtual here to avoid claiming vtable slots we have not resolved; the
    //     callers dispatch them virtually. ---
    void            NoticeRemoveOtherGroup(aiCOORD_GROUP *removed); // vtbl; forwards to members
    const dsSTRID  *GetTypeStrID();                                 // group type id
    bool            IsValidSslObj();                                // ssl-object still alive?
    // 0x832C6C80 ?ProcessFrame@aiCOORD_GROUP@@UAAXM@Z — per-frame group update (dispatched
    // virtually by aiCOORDINATOR::ProcessFrame; declared non-virtual to avoid claiming an
    // unresolved vtable slot). body: next frontier.
    void            ProcessFrame(float dt);
    // 0x832C6930 ?NoticeUnusedWP@aiCOORD_GROUP@@IAAXF@Z — mark waypoint `wpid` freed by a member.
    void            NoticeUnusedWP(short wpid);
    // 0x832C00A8 ?GetBrain@aiCOORD_GROUP@@UBAPBVaiBRAIN@@H@Z — the AI brain of member `index`
    // (dispatched virtually; declared non-virtual to avoid claiming an unresolved vtable slot).
    // Used by aiPERC::ApplyGroupSelectionWarPeace.
    const aiBRAIN  *GetBrain(int index);

    // 0x83236... aiCOORD_GROUP(name, type, sslClass) — constructor invoked by
    // aiCOORDINATOR::CreateGroup after a tracked `operator new`.
    aiCOORD_GROUP(const dsTSTRING<char> *groupName, const dsSTRID *groupType,
                  const sslCLASS_REF *sslGrpClass);

    // enemy-dist / curve / target ssl wrappers (this batch: 0x83231298..0x832321F8)
    void cbFilterWPSetByDistToEnemyLessDist(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83231298
    void cbFilterWPSetByDistToEnemySameDist(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83231388
    void cbFilterWPSetByDistFromBrain(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x83231478
    void cbFilterWPSetByDistFromUsedWPs(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);     // 0x83231568
    void cbFilterWPSetByMinHeight(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);           // 0x83231658
    void cbFilterWPSetByTargetHOPlane(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x832316F0
    void cbAddPossibleEnemiesFromTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);     // 0x83231780
    void cbSetTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);            // 0x83231DB0
    void cbAddNSToIgnoreList(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);    // 0x83231E68
    void cbRemoveNSFromIgnoreList(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83231F20
    void cbSetDistFnCurve(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x83231FD8
    void cbSetAngleFnCurve(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);      // 0x83232060
    void cbIsEqualCurves(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);        // 0x832320E8
    void cbIsValidFront(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);         // 0x832321F8

    // front / excluder / squad-actor ssl wrappers (this batch: 0x832322B0..0x83232720)
    void cbIsNeedFrontUpdate(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);        // 0x832322B0
    void cbAddFrontToExcluders(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);      // 0x83232368
    void cbRemoveFrontFromExcluders(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller); // 0x83232420
    void cbGetWPSetFromFront(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);        // 0x832324D8
    void cbFilterWPSetByFront(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);       // 0x83232590
    void cbGetSquadActor(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);            // 0x83232648

    // Remaining ssl wrappers referenced by RegisterSslFuncCb (decls only; bodies are other
    // batches / the next frontier). Uniform ssl callback ABI.
    void cbDestroyGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbClearNSIgnoreList(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbStoreTargetPosForDeltaCalc(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbClearWPSet(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbEnableReuseWPSet(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbDisableReuseWPSet(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbClearExcluders(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbSmokeOutEnemyFromHO(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // --- built-in group-class ids (static const dsSTRID, ?GRP_*@aiCOORD_GROUP@@2VdsSTRID@@B) ---
    static const dsSTRID GRP_BASE;
    static const dsSTRID GRP_SQUAD;
    static const dsSTRID GRP_ATTACK;
    static const dsSTRID GRP_DEFEND;
    static const dsSTRID GRP_FOLLOW;
    static const dsSTRID GRP_MELEE;
    static const dsSTRID GRP_CHASE;

    // 0x83232750 — static: register every aiCOORD_GROUP ssl callback on `sslClass`. Returns false
    // on the first AddCbFunc failure. ?RegisterSslFuncCb@aiCOORD_GROUP@@SA_NAAVsslCLASS_REF@@@Z
    static bool RegisterSslFuncCb(sslCLASS_REF &sslClass);
};
