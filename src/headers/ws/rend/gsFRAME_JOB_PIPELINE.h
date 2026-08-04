#pragma once
#include "../ap/apSTATE_T.h"
#include "../jbm/jbmJOB.h"
#include "../jbm/jbmJOB_EX.h"
#include "../jbm/jbmJOB_EX_WAIT_JOB.h"
#include "../jbm/jbmMANAGER.h"
// ws-engine rend/gs: the per-frame job pipeline described in CLAUDE.md's "Frame Job Pipeline"
// table — one jbmJOB (or jbmJOB_EX) global instance per named stage, routed through
// jbmMANAGER::AddJob/WaitThreadsDone by hcexCallFrame. Only the concrete fields hcexCallFrame
// reads/writes are modeled; each job's Execute()/PreExecute()/PostExecute() bodies are
// boundary (virtual, outside this re-source) — this header exists to give each global its
// DB-verified data layout, not its behavior.

struct animINST;         // boundary — model instance (mdl subsystem)
#include "rendCAM_LIST.h" // rendCAM_LIST — camera list (rend subsystem); huge (47136B)
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dBOX.h"
struct osHANDLE_DUMMY;   // boundary — os handle wrapper

// --- simple jbmJOB-derived stage markers (no extra state; DB size == base jbmJOB, 12B) ---
struct gsJBM_JOB_PRE_PROCESS_FRAME  : jbmJOB {};
struct gsJBM_JOB_PROCESS_FRAME      : jbmJOB {};
struct gsJBM_JOB_POST_PROCESS_FRAME : jbmJOB {};
struct gsJBM_JOB_STRID_UPDATE       : jbmJOB {};
struct gsJBM_JOB_VALIDATE_DYN_COLL  : jbmJOB {};
struct gsJBM_JOB_UNBIND_VID_DRV     : jbmJOB {};
struct gsREND_JOB_UNPREPARE_RENDER  : jbmJOB {};
struct gsREND_JOB_SR_DISTRIBUTE     : jbmJOB {};
struct gsREND_JOB_PUSH_PART_PREPARE : jbmJOB {};

// gsREND_JOB_UPDATE_STREAMING — jbmJOB_EX base (waitable), no extra fields (size 20).
struct gsREND_JOB_UPDATE_STREAMING : jbmJOB_EX {};

// gsREND_JOB_PREPARE_RENDER — the per-frame visibility/culling prep stage.
// DB-verified layout: jbmJOB_EX@0 (20B, base), camList@32 (rendCAM_LIST, opaque/huge),
// initialPrepare@47168 (int), doPrepareFromProcessFrame@47172 (int, function pointer used as a
// bool-ish "which prep path" selector — set to gsREND_JOB_PREPARE_RENDER::stdRendPrepareFromProcessFrame
// or 0), camListReady@47176 (osHANDLE_DUMMY*) — size 47180.
struct gsREND_JOB_PREPARE_RENDER : jbmJOB_EX {
    unsigned char _pad14[12];       // 0x014 db-verified gap (jbmJOB_EX pads to 32)
    rendCAM_LIST  camList;          // 0x020 (47136B, see rendCAM_LIST.h)
    int           initialPrepare;              // 0xB840
    int           doPrepareFromProcessFrame;   // 0xB844 fn-ptr / 0 selector
    osHANDLE_DUMMY *camListReady;              // 0xB848

    static void stdRendPrepareFromProcessFrame(); // 0x8271... boundary (outside this re-source)
};

// gsREND_JOB_PUSH_UI — pushes the UI command buffer.
struct gsREND_JOB_PUSH_UI : jbmJOB {
    int isRenderUI; // 0x0C
};

// gsREND_JOB_RENDER_FRAME — GPU command buffer submission stage.
struct gsREND_JOB_RENDER_FRAME : jbmJOB {
    int isPaused;      // 0x0C
    int doSwapBuffers; // 0x10
    int isRenderFrame; // 0x14
    int isRenderUI;    // 0x18
};

// gsREND_JOB_PUSH_PART / gsREND_JOB_PUSH_TRAIL — particle/trail renderer stage, sharded by a
// (start, freq, offset) index triple (each instance owns a distinct shard of the frame's work).
struct gsREND_JOB_PUSH_PART : jbmJOB {
    int start;  // 0x0C
    int freq;   // 0x10
    int offset; // 0x14
};
struct gsREND_JOB_PUSH_TRAIL : jbmJOB {
    int start;  // 0x0C
    int freq;   // 0x10
    int offset; // 0x14
};

// rendJOB_PUSH_INST — instanced-geometry push stage (jbmJOB_EX base; Reset()/SetGrassInst() are
// static, called without a `this`; per-instance state beyond the base is not touched here).
struct rendJOB_PUSH_INST : jbmJOB_EX {
    static void Reset();                        // 0x8285... boundary
};

struct objOBJ; // ws-engine object (pointer only here)

// rendJOB_PUSH_STAT — static-mesh push stage. DB-verified layout (types_members
// rendJOB_PUSH_STAT), size 44: jbmJOB_EX@0 (20B base), queueMT_ID@20, checkLightID@24,
// nObj@28, ppObjList@32, pObjBBOXList@36, pCamList@40.
struct rendJOB_PUSH_STAT : jbmJOB_EX {
    int           queueMT_ID;    // 0x14
    bool          checkLightID;  // 0x18
    unsigned char _pad19[3];     // 0x19
    int           nObj;          // 0x1C
    objOBJ      **ppObjList;      // 0x20
    m3dBOX       *pObjBBOXList;   // 0x24
    rendCAM_LIST *pCamList;       // 0x28
};

// gsREND_JOB_PUSH_STAT — the ws pipeline's static-mesh push job (identical to rendJOB_PUSH_STAT).
struct gsREND_JOB_PUSH_STAT : rendJOB_PUSH_STAT {};

// gsREND_JOB_PUSH_STAT_SM — skinned-mesh variant. DB-verified (types_members
// gsREND_JOB_PUSH_STAT_SM), size 84: rendJOB_PUSH_STAT@0, staticObjects@44
// (dsVECTOR<objOBJ*,16>), bboxs@64 (dsVECTOR<m3dBOX,16>).
struct gsREND_JOB_PUSH_STAT_SM : rendJOB_PUSH_STAT {
    dsVECTOR<objOBJ *, 16> staticObjects; // 0x2C
    dsVECTOR<m3dBOX, 16>   bboxs;          // 0x40
};

// rendJOB_VALIDATE_INST — one dynamic-collision-validate work item inside the manager's ring.
// DB-verified layout: jbmJOB@0 (12B), pCamList@12 (rendCAM_LIST*), nInst@16 (int),
// instList@32 (animINST*[50], 200B); array element stride is 240B (12000/50), so 8B of
// trailing padding follows instList.
struct rendJOB_VALIDATE_INST : jbmJOB {
    rendCAM_LIST *pCamList;      // 0x0C
    int           nInst;         // 0x10
    unsigned char _pad14[12];    // 0x14 (gap to instList per DB offset)
    animINST     *instList[50];  // 0x20
    unsigned char _pad_tail[8];  // 0xE8 trailing pad to the DB-verified 240B stride
};

// rendJOB_VALIDATE_INST_MANAGER — owns a fixed ring of 50 rendJOB_VALIDATE_INST work items.
// DB-verified layout: curJob@0 (int), curJob2Store@4 (int), jobs@16 (rendJOB_VALIDATE_INST[50],
// 12000B) — size 12016.
typedef struct rendJOB_VALIDATE_INST_MANAGER {
    int curJob;                            // 0x00
    int curJob2Store;                      // 0x04
    unsigned char _pad08[8];                // 0x08 (gap to jobs per DB offset)
    rendJOB_VALIDATE_INST jobs[50];        // 0x10

    void Clear();                          // 0x82758298 — reset curJob/curJob2Store to restart the ring
} rendJOB_VALIDATE_INST_MANAGER;

// rnsJOB_DIP_SORT — render-node-sort bucket job (jbmJOB_EX base + a queue id).
struct rnsJOB_DIP_SORT : jbmJOB_EX {
    int queueMT_ID; // 0x14
};

// rnsJOB_DIP_MERGE — merges sorted RNS buckets (jbmJOB_EX base + its own apSTATE_T).
struct rnsJOB_DIP_MERGE : jbmJOB_EX {
    apSTATE_T<unsigned long> state; // 0x14
};

// rnsJOB_DIP_SWAP_CLEAR — clears/swaps the RNS double buffer (no extra state).
struct rnsJOB_DIP_SWAP_CLEAR : jbmJOB_EX {};

// --- global job instances (one per named pipeline stage; see CLAUDE.md Frame Job Pipeline) ---
extern jbmMANAGER                    gsJobManager;

extern gsJBM_JOB_PRE_PROCESS_FRAME   gsJobPreProcessFrame;
extern gsJBM_JOB_PROCESS_FRAME       gsJobProcessFrame;
extern gsJBM_JOB_POST_PROCESS_FRAME  gsJobPostProcessFrame;
extern gsJBM_JOB_STRID_UPDATE        gsJobStridUpdate;
extern gsJBM_JOB_VALIDATE_DYN_COLL   gsJobValidateDynColl;
extern gsJBM_JOB_UNBIND_VID_DRV      gsJobUbindVidDrv;
extern gsREND_JOB_UNPREPARE_RENDER   gsJobUnprepareRender;
extern gsREND_JOB_SR_DISTRIBUTE      gsJobSrDistribute;
extern gsREND_JOB_PUSH_PART_PREPARE  gsJobPushPartPrepare;
extern gsREND_JOB_UPDATE_STREAMING   gsJobUpdateStreaming;
extern jbmJOB_EX_WAIT_JOB            gsJobUpdateStreamingWait;
extern gsREND_JOB_PUSH_STAT          gsJobPushStat;
extern gsREND_JOB_PUSH_STAT_SM       gsJobPushStatSM;
extern jbmJOB_EX_WAIT_JOB            gsJobWaitJobRNSSwapClearDone;
extern gsREND_JOB_PREPARE_RENDER     gsJobPrepareRender;
extern gsREND_JOB_PUSH_UI            gsJobPushUI;
extern gsREND_JOB_RENDER_FRAME       gsJobRenderFrame;

extern rendJOB_PUSH_INST             gsJobPushInst0, gsJobPushInst1, gsJobPushInst2,
                                      gsJobPushInst3, gsJobPushInst5;

extern gsREND_JOB_PUSH_PART          gsJobPushPart5_0, gsJobPushPart5_1, gsJobPushPart5_2,
                                      gsJobPushPart5_3, gsJobPushPart5_4;
extern gsREND_JOB_PUSH_TRAIL         gsJobPushTrail5_0, gsJobPushTrail5_1, gsJobPushTrail5_2,
                                      gsJobPushTrail5_3, gsJobPushTrail5_4;

extern rendJOB_VALIDATE_INST_MANAGER gsJobValidateInstMngr;

extern rnsJOB_DIP_SORT               gsJobRNSSort[6];
extern rnsJOB_DIP_MERGE              gsJobRNSMergeColor, gsJobRNSMergeSM, gsJobRNSMergeDepth,
                                      gsJobRNSMergeAll;
extern jbmJOB_EX_WAIT_JOB            gsJobRNSMergeColorWait, gsJobRNSMergeDepthWait;
extern rnsJOB_DIP_SWAP_CLEAR         gsJobRNSSwapClear;
