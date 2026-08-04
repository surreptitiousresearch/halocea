#include "../../headers/ws/gts/SYSTEM.h"
#include "../../headers/ws/ds/dsEVENT_MGR.h"

// ---- boundary declarations ----------------------------------------------------------------
extern "C" int osLockedIncrement(int *value); // boundary — atomic increment (os layer)
extern "C" int osLockedDecrement(int *value); // boundary — atomic decrement (os layer)
void operator delete(void *p);                // boundary — global delete (ws build)

// ?gEventMgr@@3PAVdsEVENT_MGR@@A — process-wide event manager. boundary global.
extern dsEVENT_MGR *gEventMgr;

// dsSMART_PTR "unresolved slot" sentinel (a pointee value of -4): such a cell is neither
// null nor a live object, so it is skipped by the ref-count inc/dec machinery.
static gts::TASK *const SMART_PTR_UNRESOLVED = (gts::TASK *)-4;

// 0x82783E88 (gts::SYSTEM::Think). Two passes over taskList:
//   Pass 1 — for each task, take an owning reference (so it survives a self-removal), then:
//     * TS_WAITING    -> TryToStart it,
//     * TS_PROCESSING -> if not ready yet, Think() it; if (now) ready, mark it TS_FINISHED and
//                        signal its ready-event through gEventMgr,
//     and drop the owning reference (destroying the task on the last release).
//   Pass 2 — unlink and free every cell whose task reached TS_FINISHED.
void gts::SYSTEM::Think()
{
    // ---- Pass 1: advance / tick every task ----
    for (ds::LIST<gts::TASK_PTR>::DATA *node = this->taskList.head; node; node = node->next)
    {
        gts::TASK *task = node->val.pointee;

        // Owning temp reference for the duration of this tick.
        gts::TASK *held = nullptr;
        if (task && task != SMART_PTR_UNRESOLVED)
        {
            osLockedIncrement(&task->refCount.refCount);
            held = task;
        }

        if (held)
        {
            if (held->state == gts::TS_WAITING)
            {
                gts::TASK_PTR tmp;      // shares the reference already taken above
                tmp.pointee = held;
                this->TryToStart(&tmp);
            }
            else if (held->state == gts::TS_PROCESSING)
            {
                if (!held->__vftable->IsReady(held))
                    held->__vftable->Think(held);

                if (held->__vftable->IsReady(held))
                {
                    held->state = gts::TS_FINISHED;
                    int readyEvent = held->__vftable->GetReadyEventID(held);
                    gEventMgr->SignalEvent(readyEvent, nullptr);
                }
            }
        }

        // Drop the owning reference; destroy on last release.
        if (held && held != SMART_PTR_UNRESOLVED)
        {
            int rc = osLockedDecrement(&held->refCount.refCount);
            if (rc <= 0)
                held->__vftable->dtr_TASK(held, 1); // scalar-deleting dtr: deleteFlag=1 => free
        }
    }

    // ---- Pass 2: reap finished tasks ----
    ds::LIST<gts::TASK_PTR>::DATA *node = this->taskList.head;
    while (node)
    {
        gts::TASK *task = node->val.pointee;

        gts::TASK *held = nullptr;
        if (task && task != SMART_PTR_UNRESOLVED)
        {
            osLockedIncrement(&task->refCount.refCount);
            held = task;
        }

        if (held && held->state == gts::TS_FINISHED)
        {
            ds::LIST<gts::TASK_PTR>::DATA *next = node->next;

            // Unlink from the doubly-linked list, fixing up head/tail as needed.
            if (node->prev)
                node->prev->next = node->next;
            else
                this->taskList.head = node->next;

            if (node->next)
                node->next->prev = node->prev;
            else
                this->taskList.tail = node->prev;

            // Release the cell's owned reference and destroy the task on last release.
            gts::TASK *cellTask = node->val.pointee;
            if (cellTask && cellTask != SMART_PTR_UNRESOLVED)
            {
                int rc = osLockedDecrement(&cellTask->refCount.refCount);
                if (rc <= 0)
                    node->val.pointee->__vftable->dtr_TASK(node->val.pointee, 1); // scalar-deleting dtr: deleteFlag=1 => free
            }
            node->val.pointee = nullptr;
            operator delete(node);

            node = next;
            --this->taskList.length;
        }
        else
        {
            node = node->next;
        }

        // Drop the pass-2 owning reference.
        if (held && held != SMART_PTR_UNRESOLVED)
        {
            int rc = osLockedDecrement(&held->refCount.refCount);
            if (rc <= 0)
                held->__vftable->dtr_TASK(held, 1); // scalar-deleting dtr: deleteFlag=1 => free
        }
    }
}
