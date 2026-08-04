#pragma once
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
// ws-engine ai08: a single behaviour object living on a brain's best-behaviour stacks.
// DB-verified layout (types_members bhvBEHAV) — size 108. Only the members reached by the aiBEST
// stack-management batch are typed (pBrain, state, typeBehav, SLID); the trailing sub-blocks
// (spDesc dsSMART_PTR@0x50, failNotifyAcceptorsSNum dsVECTOR@0x54, bhvSNum@0x68) are kept
// byte-accurate opaque so the size stays exact. Its methods reached here are virtual boundaries.

struct aiBRAIN;            // ai08 — owning brain (fwd)
struct entENTITY;          // ent — entity (fwd, NoticeGrenade arg)
struct m3dV;               // m3d — vector (fwd, GetRenderDebug args)
struct bhvBEHAV_vtbl;      // vtable (fwd)
struct bhpPARAMS;          // bhp — base behaviour params (fwd, Init arg)

// ?BHV_ST — per-behaviour state bitflags (DB types_enum_values BHV_ST).
enum BHV_ST {
    BHV_ST_SUBBEHAV          = 1,
    BHV_ST_ACTIVATED         = 2,
    BHV_ST_FINISHED          = 4,
    BHV_ST_FAILED            = 8,
    BHV_ST_END_EVENTS_SENT   = 16,
    BHV_ST_MANUALLY_CANCELLED= 32,
    BHV_ST_DONE_TERM_SEQ     = 64,
    BHV_ST_AFRAID_TO_STAY    = 256,
    BHV_ST_SHOOT_ENABLED     = 65536,
    BHV_ST_SHOOT_ON_VISIBLE  = 131072,
    BHV_ST_SHOOT_INVIS       = 262144,
    BHV_ST_QUEUE_STRIKES     = 524288,
};

struct bhvBEHAV {
    bhvBEHAV_vtbl      *__vftable;   // 0x00 — explicit-vptr convention: method decls below
                                     // are deliberately NON-virtual (cdtREFINE precedent;
                                     // a `virtual` here would add a second, implicit vptr)
    aiBRAIN            *pBrain;      // 0x04
    dsFLAGS<BHV_ST,int> state;       // 0x08
    int                 typeBehav;   // 0x0C aiBID (int to avoid the boundary enum header)
    char                SLID[64];    // 0x10 descriptor name (script-level id)
    unsigned char       spDesc[4];                 // 0x50 dsSMART_PTR<dscDESC,...> (opaque)
    dsVECTOR<unsigned long, 8> failNotifyAcceptorsSNum; // 0x54 fail-notify acceptor SNums (DB-typed)
    unsigned int        bhvSNum;                   // 0x68 behaviour context serial (timer tag)

    // ---- reached virtual methods (bodies external to this batch) — boundaries ----
    void Init(bhpPARAMS *params);                // vtbl+0x08
    void Term();                                 // vtbl+0x0C
    void Cancel();                               // vtbl+0x14
    void Fail();                                 // vtbl+0x18
    void NoticeLeftStackTop();                   // vtbl+0x44
    void ProcessFrameBeforeBody();               // vtbl+0x24
    void ProcessFrameAfterBody();                // vtbl+0x28
    void ProcessBackgroundFrame();               // vtbl+0x34
    dsTSTRING<char> GetName();                   // vtbl+? (sret) — behaviour display name
    void UpdateTimers(float dt);                 // vtbl+0x38
    void OnTimer(int timerID);                   // vtbl+0x70 — a pool timer fired for this behaviour
    void IssueBodyCmd_ToBegin();                 // vtbl+0xD0
    void IssueBodyCmd_ToEnd();                   // vtbl+0xD4
    void NoticeActive();
    void NoticeOnStackTop();
    void NoticeGrenade(entENTITY *grenade);
    bool AddSSLFuncCall(const dsTSTRING<char> &bhvPrefix, const dsTSTRING<char> &nameFunc);

    // vtbl+0xA4 (byte offset 164) — true when this behaviour currently offers any available strike.
    // Used by aiBRAIN::implHasAnyAvailableStrike.  boundary — body external to this batch.
    bool HasAnyAvailableStrike();

    // Fetch the behaviour's debug line endpoints (`from`/`to`) and its debug colour; returns whether
    // there is anything to draw this frame. Consumed by the ai08 debug renderers (a8_debug.cpp).
    // (Was previously a competing stub in aiDEBUG_render_boundaries.h; consolidated here.)
    bool GetRenderDebug(m3dV *from, m3dV *to, unsigned int *color);
};
