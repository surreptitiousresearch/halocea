// hcexJOB_SYNC_INST_MNG -- owns the fixed pool of hcexJOB_SYNC_INST workers and the shared
// animINST worklist. See src/headers/hcex/hcexJOB_SYNC_INST_MNG.h for the DB-verified layout.

#include "../../headers/hcex/hcexJOB_SYNC_INST_MNG.h"
#include "../../headers/ws/rend/gsFRAME_JOB_PIPELINE.h"
#include "../../headers/ws/scn/scnSCENE.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

extern osHANDLE_DUMMY *osCreateSignal(int isManualReset, int isInitialSignaled); // boundary
extern void            osResetSignal(osHANDLE_DUMMY *handle);                   // real body: src/ws/os/osResetSignal.cpp
extern void             osSetSignal(osHANDLE_DUMMY *handle);                     // real body: src/ws/os/osSetSignal.cpp
extern int              osWaitForSignal(osHANDLE_DUMMY *handle, unsigned long msTimeout); // real body: src/ws/os/osWaitForSignal.cpp
extern void              osPIXAddMarker(const char *string);                     // boundary
extern "C" int sprintf_0(char *string, const char *format, ...);         // real body: src/sprintf_0.c

extern scnSCENE *gsScenePtr;          // boundary -- the active Blam scene
extern int        hcex_cur_render_mode;
extern bool       hcex_obj_sync_matrices_in_progress; // boundary -- gates AddInst/StartSync/StopSync

// HCEX_OBJ::state2 bit set on animINST instances whose render matrices still need syncing this
// frame; no DB-verified name exists for this bit, so it is kept as a raw hex mask per project
// convention (do not invent a name for it).
#define ANIMINST_STATE2_NEEDS_SYNC 0x200000

// 0x823D2D1C
hcexJOB_SYNC_INST_MNG::hcexJOB_SYNC_INST_MNG()
{
    this->objIdxSyncMatr = 0;
    this->instListStart = 0;
    this->instListEnd = 0;
    this->instBatchSize = 50;
    this->lastUsedJob = 0;
    this->jobs.nEntry = 4;
    this->stopJobs = osCreateSignal(1 /*manual reset*/, 0 /*initially clear*/);
}

// 0x823D0E8C -- DEVIATION: see the header comment -- this is really the per-element array
// destructor for `jobs.list[4]` (walked back-to-front), not a write through a mistyped
// hcexJOB_SYNC_INST_MNG* at 1628-byte strides. Reproduced directly against `jobs.list` instead.
hcexJOB_SYNC_INST_MNG::~hcexJOB_SYNC_INST_MNG()
{
    for (int i = 4; i > 0; --i)
        this->jobs.list[i - 1].__vftable = (jbmJOB_vtbl *)&jbmJOB::vftable;
}

// 0x823D2D80
bool hcexJOB_SYNC_INST_MNG::AddInst(animINST *pInst)
{
    if (this->jobs.nEntry <= 1 || !hcex_obj_sync_matrices_in_progress)
        return false;

    if (!IGNORE_STRONG_ASSERT && this->instListEnd >= 2500 /* SCN_MAX_INST */)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "instListEnd < SCN_MAX_INST", "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp",
            1217, empty_string);

    bool shouldDispatch;
    if (!pInst)
    {
        shouldDispatch = true; // flush whatever range is pending, queuing nothing new
    }
    else
    {
        this->instList[this->instListEnd] = pInst;
        int newEnd = this->instListEnd + 1;
        int pendingCount = newEnd - this->instListStart;
        this->instListEnd = newEnd;
        shouldDispatch = pendingCount >= this->instBatchSize;
    }

    if (shouldDispatch)
    {
        // Pick the next worker: starting at `lastUsedJob + 1`, take the first whose `ready`
        // signal is already set (JobDone), wrapping via modulo `jobs.nEntry`; if none are free
        // by the time the scan wraps, fall back to the plain round-robin successor (never worker 0,
        // which runs inline from StopSync).
        int candidate = 1;
        int chosenJob;
        if (this->jobs.nEntry > 1)
        {
            bool foundFree = false;
            while (candidate < this->jobs.nEntry)
            {
                if (!osWaitForSignal(this->jobs.list[candidate].ready, 0))
                {
                    foundFree = true;
                    break;
                }
                ++candidate;
            }
            if (foundFree)
            {
                chosenJob = candidate;
            }
            else
            {
                int nEntry = this->jobs.nEntry;
                int next = this->lastUsedJob + 1;
                this->lastUsedJob = next % nEntry;
                if (!(next % nEntry))
                    this->lastUsedJob = 1;
                chosenJob = this->lastUsedJob;
            }
        }
        else
        {
            int nEntry = this->jobs.nEntry;
            int next = this->lastUsedJob + 1;
            this->lastUsedJob = next % nEntry;
            if (!(next % nEntry))
                this->lastUsedJob = 1;
            chosenJob = this->lastUsedJob;
        }

        int instListEnd = this->instListEnd;
        int instListStart = this->instListStart;
        this->jobs.list[chosenJob].AddInstJob(instListStart, instListEnd);
        this->instListStart = this->instListEnd;
    }
    return true;
}

// 0x823D4B38
bool hcexJOB_SYNC_INST_MNG::PushInst(animINST *pInst)
{
    return hcexJobSyncMng.AddInst(pInst);
}

// 0x823D4B60
void hcexJOB_SYNC_INST_MNG::StartSync()
{
    this->objIdxSyncMatr = 0;
    this->instListStart = 0;
    this->instListEnd = 0;
    this->lastUsedJob = 0;
    hcex_obj_sync_matrices_in_progress = true;
    osResetSignal(hcexJobSyncMng.stopJobs);

    for (int i = 0; i < this->jobs.nEntry; ++i)
    {
        this->jobs.list[i].batchIdx = 0;
        this->jobs.list[i].batchNum = 0;
    }

    if (!IGNORE_STRONG_ASSERT && this->jobs.nEntry <= 1)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "IsValidIdx(idx)", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166,
            empty_string);
    gsJobManager.AddJob(apSTATE_T<unsigned long>{4u}, &this->jobs.list[1]);

    if (!IGNORE_STRONG_ASSERT && this->jobs.nEntry <= 2)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "IsValidIdx(idx)", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166,
            empty_string);
    gsJobManager.AddJob(apSTATE_T<unsigned long>{0x10u}, &this->jobs.list[2]);

    if (!IGNORE_STRONG_ASSERT && this->jobs.nEntry <= 3)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "IsValidIdx(idx)", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166,
            empty_string);
    gsJobManager.AddJob(apSTATE_T<unsigned long>{0x20u}, &this->jobs.list[3]);

    for (animINST *inst = gsScenePtr->FindNextInst(nullptr); inst; inst = gsScenePtr->FindNextInst(inst))
    {
        if (inst->sign && !inst->pEnt && (inst->state2 & ANIMINST_STATE2_NEEDS_SYNC) != 0)
            hcexJobSyncMng.AddInst(inst);
    }
}

// 0x823D4D64
void hcexJOB_SYNC_INST_MNG::StopSync()
{
    hcexJobSyncMng.AddInst(nullptr);

    if (hcex_cur_render_mode && gsJobRenderFrame.isRenderFrame)
        gsJobManager.WaitThreadsDone(apSTATE_T<unsigned long>{0xAu});

    osSetSignal(this->stopJobs);

    if (!IGNORE_STRONG_ASSERT && this->jobs.nEntry <= 0)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "IsValidIdx(idx)", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h", 166,
            empty_string);
    this->jobs.list[0].Execute(0); // worker 0 never gets a job-manager dispatch; run it inline

    gsJobManager.WaitThreadsDone(apSTATE_T<unsigned long>{0x34u});
    hcex_obj_sync_matrices_in_progress = false;

    // DEVIATION: the decompiler rendered this as a max(0, instListEnd-instListStart) bit-hack;
    // instListEnd is only ever advanced to be >= instListStart, so the plain difference is
    // equivalent for every reachable state.
    int instLeft = this->instListEnd - this->instListStart;
    char marker[96];
    sprintf_0(marker, "inst left %d/%d", instLeft, this->instListEnd);
    osPIXAddMarker(marker);
}
