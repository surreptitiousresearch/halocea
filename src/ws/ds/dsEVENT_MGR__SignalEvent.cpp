#include "dsEVENT_MGR.h"
#include "dsPARAM_LIST.h"
#include "dsVECTOR_PARAM_LIST.h"
#include "dsPARAM.h"
#include "dsDATA.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

extern "C" void osMemoryBarrier(); // boundary — os subsystem full memory fence

// boundary — the running thread's slot index (0..7). The original reads it inline from the r13
// thread-control block (*(*(r13) + 24)); modeled here as a leaf accessor.
extern int dsEVENT_MGR_GetThreadId();

// dsEVENT_MGR::SignalEvent @ 0x82618E88
// Raise the event whose item handle is `id` on the calling thread. The per-thread pending chain is
// kept with a trailing empty sentinel: this fills the current sentinel with the event data, pops a
// fresh sentinel from the per-thread free EVENT list (refilling it with 32 events under the lock when
// exhausted), copies the parameter list into the filled event (MT-safe deep-copy when handlers exist
// on other threads, otherwise a shallow Set), links the filled event to the new sentinel, publishes
// the new head, and drives DispatchEvents unless the thread is already dispatching.
void dsEVENT_MGR::SignalEvent(int id, const dsPARAM_LIST *params)
{
    const unsigned int currentThreadId = (unsigned int)dsEVENT_MGR_GetThreadId();
    if (!IGNORE_STRONG_ASSERT && currentThreadId >= 8u)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "threadId >= 0 && threadId < 8",
            "D:\\Projects\\code\\common\\src.sys\\ds\\ds_event_mgr.cpp", 514, empty_string);

    dsEVENT_MGR::THREAD_INFO *callingThread = &this->threadInfo[currentThreadId];
    if (!IGNORE_STRONG_ASSERT && !callingThread->isActive)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "ti.isActive",
            "D:\\Projects\\code\\common\\src.sys\\ds\\ds_event_mgr.cpp", 516, empty_string);

    dsEVENT_MGR::ITEM_REC &item = this->items.Get(id);

    // Refill this thread's free EVENT list with 32 events (threaded through idxNext) when empty.
    if (this->threadInfo[currentThreadId].idxEventEmpty < 0) {
        this->lock.Lock(nullptr, 0);
        if (!IGNORE_STRONG_ASSERT && this->threadInfo[currentThreadId].numEmptyElements)
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "ti.numEmptyElements == 0",
                "D:\\Projects\\code\\common\\src.sys\\ds\\ds_event_mgr.cpp", 523, empty_string);
        int freeChainHead = -1;
        for (int remaining = 32; remaining; --remaining) {
            int prevHead = freeChainHead;
            freeChainHead = this->events.Alloc();
            this->events.Get(freeChainHead).idxNext = prevHead;
        }
        this->threadInfo[currentThreadId].idxEventEmpty = freeChainHead;
        this->threadInfo[currentThreadId].numEmptyElements = 32;
        this->lock.Unlock(nullptr, 0);
    }

    // Pop a fresh sentinel EVENT off the free list and clear it.
    int freshSentinelIdx = this->threadInfo[currentThreadId].idxEventEmpty;
    dsEVENT_MGR::EVENT &freshSentinel = this->events.Get(freshSentinelIdx);
    int nextFreeIdx = freshSentinel.idxNext;
    --this->threadInfo[currentThreadId].numEmptyElements;
    this->threadInfo[currentThreadId].idxEventEmpty = nextFreeIdx;
    freshSentinel.idxNext = -1;
    freshSentinel.id = -1;
    for (int slot = 0; slot < 8; ++slot)
        freshSentinel.refCounters[slot] = -1;

    // Fill the current sentinel (the chain head) with this event's data.
    dsEVENT_MGR::EVENT &filledEvent = this->events.Get(this->threadInfo[currentThreadId].idxEventHead);
    filledEvent.id = id;
    unsigned int age = this->threadInfo[currentThreadId].age + 1;
    this->threadInfo[currentThreadId].age = age;
    filledEvent.age = age;

    // Target-thread mask: a bit per thread that has a registered handler chain for this item.
    unsigned int threadMask = (item.threadInfo[0].idxHandlerChain >= 0);
    if (item.threadInfo[1].idxHandlerChain >= 0) threadMask |= 0x2u;
    if (item.threadInfo[2].idxHandlerChain >= 0) threadMask |= 0x4u;
    if (item.threadInfo[3].idxHandlerChain >= 0) threadMask |= 0x8u;
    if (item.threadInfo[4].idxHandlerChain >= 0) threadMask |= 0x10u;
    if (item.threadInfo[5].idxHandlerChain >= 0) threadMask |= 0x20u;
    if (item.threadInfo[6].idxHandlerChain >= 0) threadMask |= 0x40u;
    if (item.threadInfo[7].idxHandlerChain >= 0) threadMask |= 0x80u;
    filledEvent.threadMask = threadMask;

    if (params) {
        if ((threadMask & ~(1u << currentThreadId)) != 0) {
            // Handlers exist on other threads: deep-copy each parameter, forcing MT-safe values.
            filledEvent.params.storage.Clear();
            filledEvent.params.length = 0;
            filledEvent.params.list = nullptr;
            for (int paramIdx = 0; paramIdx < params->length; ++paramIdx) {
                dsPARAM *srcParam = &params->list[paramIdx];
                // decompiler shows extra args to IsMTSafe; the virtual takes none.
                if (!srcParam->data.type || srcParam->data.type->IsMTSafe()) {
                    // Already MT-safe: append a blank param, then copy id + value straight over.
                    dsPARAM blank;
                    blank.data.type = nullptr;
                    dsPARAM *slot = &filledEvent.params.storage.PushBack(blank);
                    slot->id.id = srcParam->id.id;
                    slot->data.SetValue(&srcParam->data);
                    int nElem = filledEvent.params.storage.nElem;
                    int idxZero = 0;
                    if (!IGNORE_STRONG_ASSERT && filledEvent.params.storage.nElem <= 0)
                        STRONG_ASSERT2_HELPER::asserd<int, int>(
                            "idx >= 0 && idx < nElem",
                            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                            "idx", idxZero, "nElem", filledEvent.params.storage.nElem);
                    filledEvent.params.length = nElem;
                    filledEvent.params.list = filledEvent.params.storage.pData;
                } else {
                    // Not MT-safe: convert into a scratch value, then copy that in.
                    dsDATA scratch;
                    scratch.type = nullptr;
                    scratch.StoreValue(srcParam->data);
                    if (!scratch.MakeMTSafe() && !IGNORE_STRONG_ASSERT)
                        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                            "0", "D:\\Projects\\code\\common\\src.sys\\ds\\ds_event_mgr.cpp",
                            574, empty_string);
                    const char *srcId = srcParam->id.id;
                    dsPARAM blank;
                    blank.data.type = nullptr;
                    dsPARAM *slot = &filledEvent.params.storage.PushBack(blank);
                    slot->id.id = srcId;
                    slot->data.SetValue(&scratch);
                    int nElem = filledEvent.params.storage.nElem;
                    int idxZero = 0;
                    if (!IGNORE_STRONG_ASSERT && filledEvent.params.storage.nElem <= 0)
                        STRONG_ASSERT2_HELPER::asserd<int, int>(
                            "idx >= 0 && idx < nElem",
                            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
                            "idx", idxZero, "nElem", filledEvent.params.storage.nElem);
                    filledEvent.params.length = nElem;
                    filledEvent.params.list = filledEvent.params.storage.pData;
                    if (scratch.type)
                        scratch.type->Destroy(&scratch.storage);
                }
            }
        } else {
            // Only this thread handles the event: a shallow per-parameter Set is enough.
            filledEvent.params.storage.Clear();
            filledEvent.params.length = 0;
            filledEvent.params.list = nullptr;
            for (int paramIdx = 0; paramIdx < params->length; ++paramIdx)
                filledEvent.params.Set(params->list[paramIdx]);
        }
    }

    // Publish: the fresh sentinel becomes the new head; the filled event links to it.
    callingThread->idxEventHead = freshSentinelIdx;
    osMemoryBarrier();
    filledEvent.idxNext = freshSentinelIdx;
    if (!callingThread->isDispatching)
        this->DispatchEvents();
}
