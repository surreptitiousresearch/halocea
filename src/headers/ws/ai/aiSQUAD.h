#pragma once
#include "../ent/entENTITY.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
#include "../ssl/sslOBJ_REF.h"
#include "../msg/msgRES.h"

// ws-engine ai08 — a squad object. DB-verified layout (types_members aiSQUAD) — size 448:
//   entENTITY@0 (444B base), state@444 (dsFLAGS<AI_SQST,int>). The squad owns an SSL scriptable
//   object (iaIACTOR::sslObject) and a per-class descriptor (iaIACTOR::spDesc, dynamic type
//   aiSQUAD_DESC — see aiSQUAD_DESC.h); its public/ssl methods dispatch per-class SSL functions
//   through sslObject.CallFunc using the ids stored in the descriptor.

enum AI_SQST; // ai08 — squad state flags (boundary). bit0 = init-pending, bit1 = dbg-paused.

struct aiCOORD_GROUP;   // ai08 coordinated group      (aiCOORD_GROUP_boundary.h)
struct navWP_SET;       // nav named waypoint set/front (navWP_SET_boundary.h)
struct sslCB_HOST;      // ssl callback host           (ssl/sslCB_HOST.h)
struct dsDATA;          // ds type-erased value        (ds/dsDATA.h)
struct msgADDR;         // msg address                 (msg/msgADDR.h)

struct aiSQUAD : entENTITY {
    dsFLAGS<AI_SQST, int> state; // 0x1BC squad state

    // Virtual: returns the squad's name by value (a ref-counted dsTSTRING; the caller drops the
    // returned buffer's reference — see aiPLANNER::GetSquadByName).
    // ?GetName@aiSQUAD@@UBA?AV?$dsTSTRING@D@@XZ @ 0x832A9600.  boundary — body external.
    dsTSTRING<char> GetName() const;

    // --- SSL-dispatch methods: each reads a per-class SSL function id out of the owned descriptor
    //     (spDesc.pointee, dynamic type aiSQUAD_DESC) and calls it through sslObject.CallFunc. ---
    void OnCreate();                    // 0x832A85B0 — sslFuncId_OnCreate
    void OnDbgPause();                  // 0x832A8630 — sslFuncId_OnDbgPause
    void OnDbgResume();                 // 0x832A86B0 — sslFuncId_OnDbgResume
    sslOBJ_REF GetGroup();             // 0x832A8730 — sslFuncId_GetGroup (returns ssl ref by value)
    float GetDistMemberToFront(int memberIdx, const dsTSTRING<char> &frontName);       // 0x832A8838
    float GetDistMemberToFrontTarget(int memberIdx, const dsTSTRING<char> &frontName); // 0x832A8A00
    bool  IsMemberInFront(int memberIdx, const dsTSTRING<char> &frontName);            // 0x832A8BC8

    // --- per-frame / message hooks ---
    void   ProcessFRAME();                                     // 0x832A8D88 (virtual override)
    msgRES ProcessMSG_INIT(int msg, void *pInfo, msgADDR *pSend); // 0x832A8EE8 (private)

    // GetGroupImpl — resolve the owned group's callback host, cast by callers to aiCOORD_GROUP*.
    // ?GetGroupImpl@aiSQUAD@@MAAPAVaiCOORD_GROUP@@XZ @ 0x832A8E18 (protected).
    aiCOORD_GROUP *GetGroupImpl();

    // --- native implementations behind the SSL dispatchers (virtual) ---
    bool  implIsMemberInFront(unsigned int memberIdx, const dsTSTRING<char> &frontName);        // 0x832A8410
    float implGetDistMemberToFront(unsigned int memberIdx, const dsTSTRING<char> &frontName);   // 0x832A8F48
    float implGetDistMemberToFrontTarget(unsigned int memberIdx, const dsTSTRING<char> &frontName); // 0x832A9210

    // SSL callback wrapper for GetGroup (uniform ssl callback ABI).  0x832A8338
    void cbGetGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // --- SSL callback wrappers registered by aiSQUAD_DESC::InitSslClass; bodies external. boundary ---
    void cbOnCreate(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbOnDbgPause(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    void cbOnDbgResume(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // --- SSL callback wrappers reversed in the saber2 drain (uniform ssl callback ABI) ---
    // 0x832A97F0 — REVERSED: src/ws/ai/aiSQUAD__cbGetDistMemberToFront.cpp.
    void cbGetDistMemberToFront(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832A98C8 — REVERSED: src/ws/ai/aiSQUAD__cbGetDistMemberToFrontTarget.cpp.
    void cbGetDistMemberToFrontTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);
    // 0x832A99A0 — REVERSED: src/ws/ai/aiSQUAD__cbIsMemberInFront.cpp.
    void cbIsMemberInFront(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller);

    // 0x832A9588 (?ProcessMsg@aiSQUAD@@UAA?AW4msgRES@@HPAXPAVmsgADDR@@@Z) — virtual message handler:
    // chain to entENTITY::ProcessMsg; on the create message (1) run OnCreate once (clearing the
    // init-pending state bit 0x1). REVERSED: src/ws/ai/aiSQUAD__ProcessMsg.cpp.
    msgRES ProcessMsg(int msg, void *pInfo, msgADDR *pSend);

    // --- 2-level-descent boundary callees (bodies are the next frontier) ---
    // ?implGetGroup@aiSQUAD@@UAA?AVsslOBJ_REF@@XZ @ 0x832A8168 — the native GetGroup (returns ref).
    sslOBJ_REF implGetGroup();
    // ?GetWPSet@aiSQUAD@@MBAPAVnavWP_SET@@PAVaiCOORD_GROUP@@ABV?$dsTSTRING@D@@AAV4@@Z @ 0x832A9660 —
    // resolve the front `frontName` to a navWP_SET for `group`; on failure writes a reason string
    // into `errOut`. const, protected.  boundary.
    navWP_SET *GetWPSet(aiCOORD_GROUP *group, const dsTSTRING<char> &frontName,
                        dsTSTRING<char> &errOut) const;

    // 0x832A7FC8 (?ProcessINIT@aiSQUAD@@UAAHPAX@Z) — virtual: per-spawn init; run entENTITY init and,
    // if an instance exists, register the squad with the AI planner. Returns nonzero on success.
    // REVERSED.
    int ProcessINIT(void *pInfo);

    // 0x832A8040 (?ProcessTERM@aiSQUAD@@UAAXXZ) — virtual: unregister the squad from the planner, then
    // run entENTITY teardown. REVERSED.
    void ProcessTERM();
};
