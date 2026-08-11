#include "dsEVENT_MGR.h"
#include "dsPARAM_LIST.h"
#include "ds_assert_boundary.h"

extern "C" void osMemoryBarrier(); // boundary — os subsystem full memory fence

// boundary — the running thread's slot index (0..7). The original reads it inline from the r13
// thread-control block (*(*(r13) + 24)); modeled here as a leaf accessor.
extern int dsEVENT_MGR_GetThreadId();

struct dsEVENT; // boundary — the read-only event view handed to subscribers

// dsEVENT_HANDLER is defined canonically in its own header (was locally redefined here → C2011).
#include "dsEVENT_HANDLER.h"

// The on-stack dsEVENT the compiler materialises for each subscriber: the event id, the subscriber's
// user id, then a read view (length+list) of the event's parameter list. Its address is passed to
// callbacks and object handlers as `const dsEVENT *`.
struct dsDISPATCH_EVENT {
    int          id;     // 0x00
    int          userId; // 0x04
    dsPARAM_LIST params; // 0x08
};

// dsEVENT_MGR::DispatchEvents @ 0x826159C8
// Drain every thread's pending EVENT chains on behalf of the calling thread. For each active target
// thread, walk the events queued for it (starting from this thread's per-target cursor, else the
// target's tail), and for each event invoke its item's per-thread handler chain: object handlers via
// OnEvent, inline callbacks directly, and pooled/poll subscribers by queueing a notification. The
// per-event notify count is stored back into the event, and the per-(source,target)-thread cursor is
// advanced. Runs inside the calling thread's re-entrancy guard (isDispatching).
void dsEVENT_MGR::DispatchEvents()
{
    const int currentThreadId = dsEVENT_MGR_GetThreadId();

    for (int targetThread = 0; targetThread < 8; ++targetThread) {
        if (!this->threadInfo[targetThread].isActive)
            continue;

        int *pCursor = &this->threadInfo[currentThreadId].idxThreadEvent[targetThread];
        int idxEvent = *pCursor;
        if (idxEvent < 0)
            idxEvent = this->threadInfo[targetThread].idxEventTail;
        if (idxEvent < 0)
            continue;

        int idxNextEvent = this->events.Get(idxEvent).idxNext;
        if (idxNextEvent >= 0) {
            do {
                dsEVENT_MGR::THREAD_INFO *dispatchThread = &this->threadInfo[currentThreadId];
                ++dispatchThread->isDispatching;

                dsEVENT_MGR::EVENT &event = this->events.Get(idxEvent);
                int notifyCount = 0;
                int idxHandler =
                    this->items.Get(event.id).threadInfo[currentThreadId].idxHandlerChain;
                if (idxHandler >= 0) {
                    do {
                        dsEVENT_MGR::ITEM_HANDLER &handler = this->handlers.Get(idxHandler);
                        if ((event.threadMask & (1 << currentThreadId)) != 0 &&
                            (targetThread != currentThreadId ||
                             (int)(event.age - handler.age) > 0)) {
                            dsDISPATCH_EVENT dispatchArgs;
                            dispatchArgs.id = event.id;
                            dispatchArgs.params = event.params; // slice dsVECTOR_PARAM_LIST -> base view
                            dispatchArgs.userId = handler.userId;

                            if (targetThread != currentThreadId) {
                                // Cross-thread delivery: every parameter must be MT-safe.
                                for (int paramIdx = 0; paramIdx < event.params.length; ++paramIdx) {
                                    if (!IGNORE_STRONG_ASSERT) {
                                        dsPARAM *param = &event.params.list[paramIdx];
                                        // decompiler shows extra args to IsMTSafe; the virtual takes none.
                                        if (param->data.type && !param->data.type->IsMTSafe())
                                            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                                                "ev.params[i].data.IsMTSafe()",
                                                "D:\\Projects\\code\\common\\src.sys\\ds\\ds_event_mgr.cpp",
                                                740, empty_string);
                                    }
                                }
                            }

                            if (handler.type != ST_HANDLER) {
                                if (handler.type == ST_CALLBACK) {
                                    handler.cbHandler((const dsEVENT *)&dispatchArgs);
                                } else if (handler.type < (unsigned int)(ST_POLL | ST_CALLBACK)) {
                                    int idxPending = dispatchThread->notifications.Alloc();
                                    dsEVENT_MGR::SUBSCRIBER_REC &subscriber =
                                        this->subscribers.Get(handler.idxSubscriber);
                                    dsEVENT_MGR::PENDING_ITEM &pending =
                                        dispatchThread->notifications.Get(idxPending);
                                    pending.idxEvent = idxEvent;
                                    pending.idxNext = subscriber.idxNotifyTail;
                                    pending.userId = handler.userId;
                                    int idxNotifyHead = subscriber.idxNotifyHead;
                                    subscriber.idxNotifyTail = idxPending;
                                    if (idxNotifyHead >= 0)
                                        subscriber.idxNotifyHead = idxPending;
                                    ++notifyCount;
                                }
                            } else {
                                handler.pHandler->OnEvent((const dsEVENT *)&dispatchArgs);
                            }
                        }
                        idxHandler = handler.idxNext;
                    } while (idxHandler >= 0);
                }

                if (notifyCount == 0)
                    osMemoryBarrier();

                event.refCounters[currentThreadId] = notifyCount;
                --dispatchThread->isDispatching;
                idxEvent = idxNextEvent;
                idxNextEvent = this->events.Get(idxEvent).idxNext;
            } while (idxNextEvent >= 0);
        }

        *pCursor = idxEvent; // advance the per-(source,target)-thread cursor to the last event seen
    }
}
