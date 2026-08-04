#pragma once
#include "aidr_DR.h"
#include "../ds/MAP.h"
#include "../ds/WEAK_PTR.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../m3d/m3dV.h"

struct aiBRAIN;               // ai/aiBRAIN.h — referenced by const pointer only
struct aiVIS_REQ_PARAMS;      // ai — virtual param types (function-pointer signatures only)
struct aiVIS_REQ_RES;
struct aiREACH_REQ_PARAMS;
struct aiREACH_REQ_RES;
struct aiFE_REQ_PARAMS;
struct aiFE_REQ_RES;
struct aiTRAJ_REQ_PARAMS;
struct aiTRAJ_REQ_RES;
struct aiDNS_PNT_REQ_PARAMS;
struct aiDNS_PNT_REQ_RES;

struct aiDEF_CDT_SYS;

// DB-verified layout (types_members aiDEF_CDT_SYS_vtbl): 24 virtual slots.
struct aiDEF_CDT_SYS_vtbl {
    void (__fastcall *dtr_aiDEF_CDT_SYS)(aiDEF_CDT_SYS *);                             // 0x00
    void (__fastcall *ProcessFrame)(aiDEF_CDT_SYS *, float);                            // 0x04
    void (__fastcall *DropAll)(aiDEF_CDT_SYS *);                                        // 0x08
    bool (__fastcall *CanPostVisibilityRequest)(aiDEF_CDT_SYS *, const aiBRAIN *);      // 0x0C
    void (__fastcall *PostVisibilityRequest)(aiDEF_CDT_SYS *, const aiVIS_REQ_PARAMS *); // 0x10
    bool (__fastcall *HaveVisibilityResults)(aiDEF_CDT_SYS *, const aiBRAIN *);         // 0x14
    void (__fastcall *GetVisibilityResults)(aiDEF_CDT_SYS *, const aiBRAIN *, aiVIS_REQ_RES *); // 0x18
    bool (__fastcall *CanPostReachRequest)(aiDEF_CDT_SYS *, const aiBRAIN *);           // 0x1C
    void (__fastcall *PostReachRequest)(aiDEF_CDT_SYS *, const aiREACH_REQ_PARAMS *);   // 0x20
    bool (__fastcall *HaveReachResults)(aiDEF_CDT_SYS *, const aiBRAIN *);              // 0x24
    void (__fastcall *GetReachResults)(aiDEF_CDT_SYS *, const aiBRAIN *, aiREACH_REQ_RES *); // 0x28
    bool (__fastcall *CanPostFindEnemyRequest)(aiDEF_CDT_SYS *, const aiBRAIN *);       // 0x2C
    void (__fastcall *PostFindEnemyRequest)(aiDEF_CDT_SYS *, const aiFE_REQ_PARAMS *);  // 0x30
    bool (__fastcall *HaveFindEnemyResults)(aiDEF_CDT_SYS *, const aiBRAIN *);          // 0x34
    void (__fastcall *GetFindEnemyResults)(aiDEF_CDT_SYS *, const aiBRAIN *, aiFE_REQ_RES *); // 0x38
    bool (__fastcall *CanPostTrajectoryRequest)(aiDEF_CDT_SYS *, const aiBRAIN *, int); // 0x3C
    void (__fastcall *PostTrajectoryRequest)(aiDEF_CDT_SYS *, const aiTRAJ_REQ_PARAMS *); // 0x40
    bool (__fastcall *HaveTrajectoryResults)(aiDEF_CDT_SYS *, const aiBRAIN *, int);    // 0x44
    void (__fastcall *GetTrajectoryResults)(aiDEF_CDT_SYS *, const aiBRAIN *, int, aiTRAJ_REQ_RES *); // 0x48
    void (__fastcall *ForgetAllTrajectoryRequests)(aiDEF_CDT_SYS *, const aiBRAIN *);   // 0x4C
    bool (__fastcall *CanPostDNSPNTRequest)(aiDEF_CDT_SYS *, const aiBRAIN *);          // 0x50
    void (__fastcall *PostDNSPNTRequest)(aiDEF_CDT_SYS *, const aiDNS_PNT_REQ_PARAMS *); // 0x54
    bool (__fastcall *HaveDNSPNTResults)(aiDEF_CDT_SYS *, const aiBRAIN *);             // 0x58
    void (__fastcall *GetDNSPNTResults)(aiDEF_CDT_SYS *, const aiBRAIN *, aiDNS_PNT_REQ_RES *); // 0x5C
};

// ws-engine ai: the deferred-request coordinator — collects strike/visibility/etc. requests during
// a frame (srcReqs), promotes them to in-flight requests (dstReqs) the next frame, ticks their
// destroy timers, and reaps completed ones. DB-verified layout (types_members aiDEF_CDT_SYS):
// __vftable@0, srcReqs@4, dstReqs@88, dbgRays@172 — size 656.
struct aiDEF_CDT_SYS {
    // Map key: which (sender-brain, request-type, sub-group) a deferred request belongs to.
    // DB-verified layout (types_members aiDEF_CDT_SYS::keyREQ): pSender@0, pSenderDbg@4, typeReq@8,
    // sgidx@12 — size 16.
    struct keyREQ {
        const aiBRAIN         *pSender;    // 0x00
        ds::WEAK_PTR<aiBRAIN>  pSenderDbg; // 0x04 weak ref (keeps the debug handle alive)
        aidr::DR_TYPE          typeReq;    // 0x08
        int                    sgidx;      // 0x0C sub-group index
    };

    // Debug ray record. DB-verified layout (types_members aiDEF_CDT_SYS::FROM_TO): from@0, to@12.
    struct FROM_TO {
        m3dV from; // 0x00
        m3dV to;   // 0x0C
    };

    typedef ds::MAP<keyREQ, aidr::DR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> ReqMap;
    typedef ds::impl::MAP_ITERATOR<keyREQ, aidr::DR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> ReqIter;
    typedef ds::impl::MAP_CONST_ITERATOR<keyREQ, aidr::DR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> ReqConstIter;

    aiDEF_CDT_SYS_vtbl        *__vftable; // 0x000
    ReqMap                     srcReqs;   // 0x004 requests accumulated this frame
    ReqMap                     dstReqs;   // 0x058 in-flight requests (promoted from srcReqs)
    dsCONST_ARRAY<FROM_TO, 20> dbgRays;   // 0x0AC debug shoot rays

    // 0x8323CA48 — true when a matching request key is present in either map.
    bool HaveRequest(const aiBRAIN *pBrain, aidr::DR_TYPE type, int sgidx) const;
    // 0x8323CB30 — true when the matching in-flight request has produced its result.
    bool HaveResults(const aiBRAIN *pBrain, aidr::DR_TYPE type, int sgidx) const;
    // 0x8323CBF8 — decay every in-flight request's destroy timer by `dt`.
    void UpdateTimers(float dt);
    // 0x8323DD60 — analyse each in-flight request; reap (Erase + delete) those whose timer expired.
    void AnalyzeResults();
    // 0x8323E840 — promote every accumulated request: produce its rays, move it into dstReqs, clear srcReqs.
    void ProcessRequests();
    // 0x83246618 (?DebugRender@aiDEF_CDT_SYS@@QAAXXZ) — draw the recorded find-enemy shoot rays when
    // the find-enemy debug toggle is on. REVERSED.
    void DebugRender();

    // Find-enemy query surface (virtual overrides; slots 0x2C-0x38 of aiDEF_CDT_SYS_vtbl).
    // ?CanPostFindEnemyRequest@aiDEF_CDT_SYS@@UBA_NPBVaiBRAIN@@@Z — true when `viewer` may post a
    // new find-enemy request this frame.
    bool CanPostFindEnemyRequest(const aiBRAIN *viewer) const;
    // ?PostFindEnemyRequest@aiDEF_CDT_SYS@@UAAXABUaiFE_REQ_PARAMS@@@Z — enqueue `req`.
    void PostFindEnemyRequest(const aiFE_REQ_PARAMS &req);
    // ?HaveFindEnemyResults@aiDEF_CDT_SYS@@UBA_NPBVaiBRAIN@@@Z — true when results for `viewer` are ready.
    bool HaveFindEnemyResults(const aiBRAIN *viewer) const;
    // ?GetFindEnemyResults@aiDEF_CDT_SYS@@UAAXPBVaiBRAIN@@AAUaiFE_REQ_RES@@@Z — copy `viewer`'s result into `out`.
    void GetFindEnemyResults(const aiBRAIN *viewer, aiFE_REQ_RES &out);
    // ?CanPostVisibilityRequest@aiDEF_CDT_SYS@@UBA_NPBVaiBRAIN@@@Z — virtual const (vtbl+0x0C):
    // true when `viewer` may post a new visibility request this frame. boundary — body external.
    bool CanPostVisibilityRequest(const aiBRAIN *viewer) const;
    // 0x8323E220 (?HaveVisibilityResults@aiDEF_CDT_SYS@@UBA_NPBVaiBRAIN@@@Z) — virtual const
    // (vtbl+0x14): true when a visibility result for `viewer` is ready. boundary — body external.
    bool HaveVisibilityResults(const aiBRAIN *viewer) const;
    // 0x8323DF90 (?GetVisibilityResults@aiDEF_CDT_SYS@@UAAXPBVaiBRAIN@@AAUaiVIS_REQ_RES@@@Z) —
    // virtual (vtbl+0x18): copy `viewer`'s visibility result into `out`. boundary — body external.
    void GetVisibilityResults(const aiBRAIN *viewer, aiVIS_REQ_RES &out);

    // Deferred-request producers (virtual overrides). Each allocates the request-type-specific
    // aidr::DR subclass, copies the params into it, and inserts it (keyed by sender/type/sub-group)
    // into srcReqs to be promoted next frame. REVERSED (a8_def_cdt_sys.cpp).
    void PostVisibilityRequest(const aiVIS_REQ_PARAMS &params);        // 0x8323EB20
    void PostReachRequest(const aiREACH_REQ_PARAMS &params);           // 0x8323E9B8
    void PostTrajectoryRequest(const aiTRAJ_REQ_PARAMS &params);       // 0x8323EEA0
    void PostDNSPNTRequest(const aiDNS_PNT_REQ_PARAMS &params);        // 0x8323F058
    // 0x8323F010 — drop every accumulated/in-flight trajectory request whose sender is `pBrain`.
    void ForgetAllTrajectoryRequests(const aiBRAIN *pBrain);
    // 0x8323F188 — per-frame tick: publish request counters, analyse results, promote+age requests.
    void ProcessFrame(float dt);
    // 0x8323DF38 (?DropAll@aiDEF_CDT_SYS@@UAAXXZ) — drop every accumulated/in-flight request.
    void DropAll();
    // 0x832466F0 (?DebugAddLine@aiDEF_CDT_SYS@@QAAXABUm3dV@@0@Z) — append a debug ray (from->to) to
    // dbgRays if there is room (max 20). REVERSED: src/ws/ai/aiDEF_CDT_SYS__DebugAddLine.cpp.
    void DebugAddLine(const m3dV &from, const m3dV &to);
};
