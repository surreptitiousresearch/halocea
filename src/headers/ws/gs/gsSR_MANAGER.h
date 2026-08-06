#pragma once
// ws-engine gs: the "spatial-refine" (collision-query) manager. It owns two per-processor
// delayed-job managers (PPU + SPU) that batch cdtREFINE collision queries and run them as
// deferred jobs, and it drives the per-frame copy/execute/notify cycle.
//
// Layout is taken from the DISASSEMBLY, which is authoritative here: the concrete methods
// (ProcessLastPortion @0x82D26A00, CopyDelayed2LastValid @0x82D27318) access the PPU int-manager
// at this+0x20 (32) and the SPU int-manager at this+0x38 (56). The DB's reconstructed
// `gsSR_MANAGER` types_members table instead lists a phantom `srMANAGER`@32 / mngPPU@64 / mngSPU@88
// that contradicts those offsets, so it is NOT followed. msgADDR occupies the first 32 bytes
// (message-system base), then the two int-managers follow contiguously (each size 24).

#include "../ds/dsVECTOR.h"
#include "../msg/msgADDR.h"      // msgADDR — ws message-system base (32 bytes)
struct gsJBM_JOB_DELAY_MNG_PPU;  // boundary — PPU delayed-job record
struct gsJBM_JOB_DELAY_MNG_SPU;  // boundary — SPU delayed-job record
struct jbmJOB;                   // boundary — frame-job record

// DB-verified layout (types_members gsSR_MANAGER_INT<...>, size 24): lastJobNmb@0, jobs@4.
template<class JOB_DELAY_MNG>
struct gsSR_MANAGER_INT {
    int                              lastJobNmb; // 0x00 index of the job started last frame
    dsVECTOR<JOB_DELAY_MNG *, 8>     jobs;       // 0x04 ring of per-frame delayed-job managers

    // 0x82D27334-region — promote this frame's delayed queries to the "last valid" snapshot.
    // boundary — distinct per-instantiation body; declared, not reversed in this batch.
    void CopyDelayed2LastValid();
};

// srMANAGER base — the abstract interface hcexCallFrame dispatches through (ProcessLastPortion /
// CheckAddExecJobs / CopyDelayed2LastValid / RunAllDelayedJobs). boundary: only RunAllDelayedJobs
// is called from the concrete method below.
// Partial layout from the disasm of gsStartJobSrMng (0x82515C7C: PushBack targets this+8):
// srJobs (the single-thread deferred-job list) lives at offset 8.
struct srMANAGER {
    void                       *_vfptr;   // 0x00 vtable pointer
    int                         _unk4;    // 0x04 (not touched by this batch)
    dsVECTOR<jbmJOB *, 8>       srJobs;   // 0x08 single-thread deferred job list

    void RunAllDelayedJobs(); // 0x82D26A48 callee — boundary (external to this batch)

    // 0x8277B1A8 ?ShootRay@srMANAGER@@QAAHABUm3dV@@0MPAVcdtREFINE@@PAVcdtINFO@@UapCL@@@Z
    // Cast a collision ray (from,dir,dist); fills pInfo, honouring the refine filter. The trailing
    // apCL is the tracked source-location cookie. boundary — body external to this batch.
    int ShootRay(const struct m3dV &from, const struct m3dV &dir, float dist,
                 struct cdtREFINE *refine, struct cdtINFO *pInfo, struct apCL cl);
};

// Global active spatial-refine manager pointer (_pSrMng@0x... : srMANAGER*).
extern srMANAGER *pSrMng;

// The concrete manager. Derives from msgADDR (message-system actor) and srMANAGER (the virtual
// interface); embeds the two int-managers at the disasm offsets. Only the msgADDR base (32B,
// [0x00,0x20)) is modeled; the DB's srMANAGER@32/mngPPU@64/mngSPU@88 rows contradict the disasm
// (see file-top comment) and are NOT followed.
struct gsSR_MANAGER : msgADDR {
    gsSR_MANAGER_INT<gsJBM_JOB_DELAY_MNG_PPU> mngPPU; // 0x20 (this+0x20 per disasm)
    gsSR_MANAGER_INT<gsJBM_JOB_DELAY_MNG_SPU> mngSPU; // 0x38 (this+0x38 per disasm)

    // 0x82D26A00 — start the last-portion PPU/SPU jobs, then run all delayed jobs.
    void ProcessLastPortion();
    // 0x82D250B8 — reset the per-frame query counters.
    void CheckAddExecJobs();
    // 0x82D27318 — copy both int-managers' delayed queries to last-valid, prune dead refine
    // handles, run their Modify(), then fire cdt-processed notifiers.
    void CopyDelayed2LastValid();
};

// srMANAGER::RunAllDelayedJobs actually lives on the srMANAGER base (see 0x82D26A48 call).
// gsStartJobSrMng(jbmJOB*) — enqueue/start one delayed-manager job. boundary.
extern void gsStartJobSrMng(jbmJOB *pJob);

// Per-frame query counters reset by CheckAddExecJobs. Module globals.
extern "C" int queryAddNmb;
extern "C" int queryExecuteNmb;
