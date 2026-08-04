// scrSCORCH_MANAGER::RemoveAll @ 0x827394E0
//
// Tears down every scorch batch and mark the manager owns, resets the global scorch statistics,
// and installs a fresh (empty) poly-collector:
//   1. For each static voxel: free its static batch/mark vectors and empty its farmed-mark deque.
//   2. For each dynamic scorch batch: release its light set, textures, per-layer material textures,
//      decal-pass texture bundle, and geometry buffers, then delete the batch.
//   3. Free the dynamic-batch pointer vector and clear the dynamic scorch list.
//   4. Allocate a new scorch::COLLECTOR and swap it into pCollector (via the shared-ptr Reset).
//   5. Zero the scrSCORCH_INFO counters.
// The whole operation runs under the global multithread lock (g_syncMT).
//
// Bounded-descent boundaries (declared extern, not re-sourced here): osLOCK::Lock/Unlock,
// osLockedDecrement, dsVECTOR::Realloc, ds::VAL_LIST::Clear / ::InsertAfter, scrTEXTURES::~scrTEXTURES,
// PASS_DESC_DECAL_TEX::~PASS_DESC_DECAL_TEX, txmTEXTURE::Release, scorch::COLLECTOR::COLLECTOR,
// dsSHARED_COUNTER::GetPool, dsSHARED_PTR::Reset, dlFree/dlFreeAligned, operator new/delete.

#include "../../headers/ws/scorch/scrSCORCH_MANAGER.h"
#include "../../headers/ws/scorch/scrSCORCH_INFO.h"
#include "../../headers/ws/scorch/scrTEXTURES.h"      // txmManager, scrTEXTURES::~scrTEXTURES
#include "../../headers/ws/rend/rendLIGHT_SET.h"
#include "../../headers/ws/ds/dsSHARED_COUNTER.h"
#include "../../headers/ws/txm/txmTEXTURE.h" // txmTEXTURE::Release
#include <new>                                 // placement new for the in-place COLLECTOR ctor

// Debug-tagged operator new (size, file, line) the ws build links in. boundary.
void *operator new(size_t size, const char *file, unsigned int line);

// ---- boundary externs (deeper than the 2-level descent budget) -------------------------------
struct osLOCK;
extern osLOCK g_syncMT;
extern "C" int osLockedDecrement(volatile int *p);
void osLOCK__Lock(osLOCK *self, void *, int);   // osLOCK::Lock
void osLOCK_Unlock(osLOCK *self, void *, int); // osLOCK::Unlock
extern void dlFree(void *ptr);
extern void dlFreeAligned(void *ptr);
// scorch::COLLECTOR ctor lives in COLLECTOR__ctor.cpp; Deleter/Reset in dsSHARED_PTR.

void scrSCORCH_MANAGER::RemoveAll()
{
    osLOCK__Lock(&g_syncMT, nullptr, 0);

    // (1) Free every static voxel's batches/marks and empty its farmed-mark ring deque.
    for (int voxIdx = 0; voxIdx < this->scrVoxels.nElem; ++voxIdx)
    {
        scrVOX *vox = &this->scrVoxels[voxIdx]; // vector stores scrVOX by value
        vox->batches.Realloc(0);
        vox->scorchmarks.Realloc(0);

        scrSCORCH_MARK_STAT_CD *farmedData = vox->scorchmarksFarmed.data.pData;
        vox->scorchmarksFarmed.data.pData    = nullptr;
        vox->scorchmarksFarmed.data.nElem    = 0;
        vox->scorchmarksFarmed.data.allocated = 0;
        vox->scorchmarksFarmed.head   = 0;
        vox->scorchmarksFarmed.tail   = 0;
        vox->scorchmarksFarmed.length = 0;
        dlFree(farmedData);
    }

    // (2) Destroy every dynamic scorch batch.
    int checkedCount = 0;
    if (this->dynScorchBatches.nElem > 0)
    {
        int batchIdx = 0;
        do
        {
            scrBATCH_DYN *batch = this->dynScorchBatches.pData[batchIdx];
            if (batch)
            {
                // Release the batch's light set (last-owner drops it via its virtual deleting dtor).
                rendLIGHT_SET *lset = batch->desc.lset.pointee;
                if (lset && lset != (rendLIGHT_SET *)-4) // -4 == not-yet-resolved smart-ptr sentinel
                {
                    int newRefCount = osLockedDecrement((volatile int *)&lset->ref.refCount);
                    if ((newRefCount | (newRefCount - 1)) < 0) // refcount reached zero
                    {
                        rendLIGHT_SET *toFree = batch->desc.lset.pointee;
                        if (toFree)
                            // scalar-deleting dtr: deleteFlag=1 (from the disassembly) => run
                            // ~rendLIGHT_SET then free. Modeled directly in the vtbl slot signature.
                            toFree->__vftable->dtr_rendLIGHT_SET(toFree, 1);
                    }
                }
                batch->desc.lset.pointee = nullptr;

                batch->desc.textures.~scrTEXTURES();

                // Release each per-layer material texture, but only while the texture manager lives.
                vidPASS_OBJ_MTL &mtl = batch->vidCfgColor.passObj.mtl;
                if (mtl.layer2.texSpec.ptr && txmManager) mtl.layer2.texSpec.ptr->Release();
                if (mtl.layer2.texNM.ptr   && txmManager) mtl.layer2.texNM.ptr->Release();
                if (mtl.layer2.texDiff.ptr && txmManager) mtl.layer2.texDiff.ptr->Release();
                if (mtl.layer1.texSpec.ptr && txmManager) mtl.layer1.texSpec.ptr->Release();
                if (mtl.layer1.texNM.ptr   && txmManager) mtl.layer1.texNM.ptr->Release();
                if (mtl.layer1.texDiff.ptr && txmManager) mtl.layer1.texDiff.ptr->Release();
                if (mtl.layer0.texSpec.ptr && txmManager) mtl.layer0.texSpec.ptr->Release();
                if (mtl.layer0.texNM.ptr   && txmManager) mtl.layer0.texNM.ptr->Release();
                if (mtl.layer0.texDiff.ptr && txmManager) mtl.layer0.texDiff.ptr->Release();
                if (mtl.lm.texDir.ptr && txmManager) mtl.lm.texDir.ptr->Release();
                if (mtl.lm.texDif.ptr && txmManager) mtl.lm.texDif.ptr->Release();
                if (mtl.base.tex.ptr  && txmManager) mtl.base.tex.ptr->Release();

                batch->vidDescPass.tex.~PASS_DESC_DECAL_TEX(); // PASS_DESC_DECAL::TEX::~TEX

                dlFreeAligned(batch->indList.pData);
                dlFreeAligned(batch->vtxList.pData);
                operator delete(batch);
            }
            ++checkedCount;
            ++batchIdx;
        }
        while (checkedCount < this->dynScorchBatches.nElem);
    }

    // (3) Drop the dynamic-batch pointer vector and the dynamic scorch list.
    this->dynScorchBatches.Realloc(0);
    this->dynScorchList.Clear();
    this->memUsedForStat = 0;
    this->memUsedForDyn  = 0;

    // (4) Allocate and install a fresh, empty poly-collector.
    scorch::COLLECTOR *newCollector =
        (scorch::COLLECTOR *)operator new(0xD4u,
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\scorch_manager.cpp", 0x3EA);
    if (newCollector)
        new (newCollector) scorch::COLLECTOR();
    else
        newCollector = nullptr;

    // Register a shared-counter slot for the new collector, then reset the shared pointer so the
    // previous collector's reference is released and the new one adopted.
    int newCounter = 0;
    dsSHARED_COUNTER counterSlot(1); // only an int ctor exists (no default); used=1
    ds::VAL_LIST<dsSHARED_COUNTER>::ITERATOR insertPos;
    if (newCollector)
    {
        counterSlot.used = 1;
        ds::VAL_LIST<dsSHARED_COUNTER> *pool = dsSHARED_COUNTER::GetPool();
        insertPos.pool    = &pool->pool;
        insertPos.elemIdx = pool->usedTail;
        pool->InsertAfter(counterSlot, insertPos);
        newCounter = dsSHARED_COUNTER::GetPool()->usedTail;
    }
    else
    {
        dsSHARED_COUNTER::GetPool();
    }

    // Build a temporary shared-ptr view over the old referent/counter and Reset() it after
    // publishing the new collector (matches the decompiler's in-place pointee/counter swap).
    scorch::COLLECTOR *oldCollector = this->pCollector.pointee;
    this->pCollector.pointee = newCollector;
    dsSHARED_PTR<scorch::COLLECTOR, 0, Deleter<scorch::COLLECTOR> > oldRef;
    oldRef.pointee = oldCollector;
    oldRef.counter = this->pCollector.counter;
    this->pCollector.counter = newCounter;
    oldRef.Reset();

    // (5) Reset the global scorch statistics.
    scrSCORCH_INFO::nScorchStat = 0;
    scrSCORCH_INFO::nScorchDyn  = 0;
    scrSCORCH_INFO::nFacesDyn   = 0;
    scrSCORCH_INFO::nFacesStat  = 0;
    scrSCORCH_INFO::nVertDyn    = 0;
    scrSCORCH_INFO::nVertStat   = 0;
    scrSCORCH_INFO::nBatchStat  = 0;
    scrSCORCH_INFO::nBatchDyn   = 0;

    osLOCK_Unlock(&g_syncMT, nullptr, 0);
}
