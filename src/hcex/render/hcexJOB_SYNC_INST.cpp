// hcexJOB_SYNC_INST -- one worker slot of the hcexJOB_SYNC_INST_MNG pool. See
// src/headers/hcex/hcexJOB_SYNC_INST.h for the DB-verified layout.

#include "../../headers/hcex/hcexJOB_SYNC_INST.h"
#include "../../headers/hcex/hcexJOB_SYNC_INST_MNG.h"
#include "../../headers/hcex/HCEX_OBJ.h"
#include "../../headers/ws/rend/gsFRAME_JOB_PIPELINE.h"
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

extern rendDRIVER *rendDrv; // global render driver (declared in hcex_fill_backbuffer_boundary.h)

void operator delete(void *ptr); // boundary -- CRT operator delete

extern osHANDLE_DUMMY *osCreateSignal(int isManualReset, int isInitialSignaled);          // boundary
extern void            osSetSignal(osHANDLE_DUMMY *handle);                              // real body: src/ws/os/osSetSignal.cpp
extern int             osWaitForSignal(osHANDLE_DUMMY *handle, unsigned long msTimeout);  // real body: src/ws/os/osWaitForSignal.cpp
extern int             osWaitForFirstOfMultipleSignals(int count, osHANDLE_DUMMY *const *ptrList,
                                                        unsigned int msTimeout);          // boundary
extern int              osLockedAdd(int *pVar, int addend);                              // real body: src/ws/os/osLockedAdd.cpp
extern void             osPIXBeginEvent(const char *name);                                // real body: src/ws/os/osPIXBeginEvent.cpp
extern void             osPIXEndEvent(void);                                              // real body: src/ws/os/osPIXEndEvent.cpp
extern "C" int sprintf_0(char *string, const char *format, ...);                   // real body: src/sprintf_0.c

extern HCEX_OBJ *hcex_obj_get_by_idx(int idx);         // real body: src/hcex/hcex_obj_get_by_idx.c
extern void       hcex_obj_sync(HCEX_OBJ *pObj);        // boundary -- not part of this batch
extern "C" void       hcex_pix_marker(const char *fmt, ...); // real body: src/hcex/hcex_pix_marker.c

extern bool rendIsCullAndValidateInstNeeded(animINST *pInst);                                  // boundary
extern void rendCullAndValidateInst(const rendCAM_LIST *camList, unsigned int curFrameNmb,
                                     float elapsedTime, animINST *pInst);                       // boundary

extern int          hcex_cur_render_mode; // boundary
extern unsigned int gsAppState2;          // boundary -- see CLAUDE.md gsAppState flag reference
#define GS_APP_STATE2_SUSPEND_FRAME_SKIP 0x10000u

// 0x823D0ED0
hcexJOB_SYNC_INST::hcexJOB_SYNC_INST()
{
    this->state.state = 0;
    this->name = "hcex_sync_obj";
    this->__vftable = (jbmJOB_vtbl *)&hcexJOB_SYNC_INST::vftable;
    this->batchIdx = 0;
    this->batchNum = 0;
    this->ready = osCreateSignal(0 /*auto reset*/, 1 /*initially signaled*/);
    this->start = osCreateSignal(0 /*auto reset*/, 0 /*initially clear*/);
}

// 0x823CE6A0 -- thunk: re-point the vtable back to the jbmJOB base before base teardown.
hcexJOB_SYNC_INST::~hcexJOB_SYNC_INST()
{
    this->__vftable = (jbmJOB_vtbl *)&jbmJOB::vftable;
}

// 0x823D0F3C
hcexJOB_SYNC_INST *hcexJOB_SYNC_INST::VectorDeletingDtor(unsigned char deleteFlags)
{
    this->__vftable = (jbmJOB_vtbl *)&jbmJOB::vftable;
    if (deleteFlags & 1)
    {
        operator delete(this);
        return this;
    }
    return this;
}

// 0x823CE708
void hcexJOB_SYNC_INST::AddInstJob(int instListStart, int instListEnd)
{
    if (!IGNORE_STRONG_ASSERT && this->batchNum >= 200)
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
            "batchNum < (sizeof(batches)/sizeof((batches)[0]))",
            "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 1336, empty_string);

    this->batches[this->batchNum].start = instListStart;
    this->batches[this->batchNum].end   = instListEnd;
    osHANDLE_DUMMY *wake = this->start;
    ++this->batchNum;
    osSetSignal(wake);
}

// 0x823DD540
void hcexJOB_SYNC_INST::Execute(int threadID)
{
    char jobEventName[64];
    sprintf_0(jobEventName, "hcex_sync_job%d", threadID);
    osPIXBeginEvent(jobEventName);

    char objEventName[64];
    sprintf_0(objEventName, "hcex_sync_obj%d", threadID);
    osPIXBeginEvent(objEventName);

    dsCONST_ARRAY<unsigned short, 2048> syncedIdx;
    syncedIdx.nEntry = 0;

    int nSynced = 0;
    int objIdx = osLockedAdd(&hcexJobSyncMng.objIdxSyncMatr, 1);
    for (HCEX_OBJ *obj = hcex_obj_get_by_idx(objIdx); obj; obj = hcex_obj_get_by_idx(objIdx))
    {
        hcex_obj_sync(obj);

        if (!IGNORE_STRONG_ASSERT && syncedIdx.nEntry >= 2048)
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
                "Length() < size", "d:\\projects\\code\\common\\incl.sys\\ds\\ds_const_list.h",
                175, empty_string);

        syncedIdx.list[syncedIdx.nEntry] = (unsigned short)objIdx;
        ++syncedIdx.nEntry;
        ++nSynced;
        objIdx = osLockedAdd(&hcexJobSyncMng.objIdxSyncMatr, 1);
    }
    hcex_pix_marker("processed %d", nSynced);
    osPIXEndEvent();

    if (gsJobRenderFrame.isRenderFrame && (gsAppState2 & GS_APP_STATE2_SUSPEND_FRAME_SKIP) != 0
        && hcex_cur_render_mode)
    {
        osPIXBeginEvent("wait for cam");
        osWaitForSignal(gsJobPrepareRender.camListReady, 0xFFFFFFFF);
        osPIXEndEvent();

        const rendCAM_LIST *camList = gsJobPrepareRender.doPrepareFromProcessFrame
            ? (const rendCAM_LIST *)&gsJobPrepareRender.camList
            : (const rendCAM_LIST *)&rendDrv->camList;

        osPIXBeginEvent("hcex_validate_inst");
        for (int i = 0; i < syncedIdx.nEntry; ++i)
        {
            unsigned short idx = syncedIdx.list[i];
            animINST *pInst = hcex_obj_get_by_idx(idx)->pInst;
            if (pInst && rendIsCullAndValidateInstNeeded(pInst))
                rendCullAndValidateInst(camList, rendDrv->curFrameNmb, rendDrv->elapsedTime, pInst);
        }
        osPIXEndEvent();

        int nInstProcessedTotal = 0;
        int nInstProcessed = 0;
        osPIXBeginEvent("hcex_sync_inst");
        osHANDLE_DUMMY *readySignal = this->ready;
        osHANDLE_DUMMY *waitSignals[2];
        waitSignals[0] = hcexJobSyncMng.stopJobs;
        waitSignals[1] = this->start;
        osSetSignal(readySignal);
        while (osWaitForFirstOfMultipleSignals(2, waitSignals, 0xFFFFFFFF) == 1)
        {
            osPIXBeginEvent("hcex_validate_inst_pak");
            int nBatch = 0;
            int nBatchProcessed = 0;
            if (this->batchIdx < this->batchNum)
            {
                do
                {
                    int batchIdx = this->batchIdx;
                    int rangeEnd   = this->batches[batchIdx].end;
                    int rangeStart = this->batches[batchIdx].start;
                    if (rangeStart < rangeEnd)
                    {
                        int nRemaining = rangeEnd - rangeStart;
                        nInstProcessedTotal += rangeEnd - rangeStart;
                        animINST **pCur = &hcexJobSyncMng.instList[rangeStart];
                        nBatch += rangeEnd - rangeStart;
                        do
                        {
                            if (rendIsCullAndValidateInstNeeded(*pCur))
                            {
                                rendCullAndValidateInst(camList, rendDrv->curFrameNmb,
                                                         rendDrv->elapsedTime, *pCur);
                                ++nInstProcessed;
                                ++nBatchProcessed;
                            }
                            --nRemaining;
                            ++pCur;
                        } while (nRemaining);
                    }
                    this->batchIdx = batchIdx + 1;
                } while (this->batchIdx < this->batchNum);
            }
            hcex_pix_marker("inst processed %d/%d", nBatchProcessed, nBatch);
            osPIXEndEvent();
            osSetSignal(this->ready);
        }
        hcex_pix_marker("inst total processed %d/%d", nInstProcessed, nInstProcessedTotal);
        osPIXEndEvent();
    }
    osPIXEndEvent();
}

// 0x823CE6C8
bool hcexJOB_SYNC_INST::JobDone()
{
    return osWaitForSignal(this->ready, 0) == 0;
}
