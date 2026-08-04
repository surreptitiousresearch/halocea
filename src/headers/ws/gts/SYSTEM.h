#pragma once
#include "TASK.h"
#include "../ds/LIST.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
// ws-engine gts (game task scheduler): owns the list of scheduled tasks and ticks them each
// frame. DB-verified layout (types_members gts::SYSTEM): taskList@0
// (ds::LIST<dsSMART_PTR<gts::TASK,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>>>) — size 20.

namespace gts {

// The owning smart pointer stored in each task-list cell.
typedef dsSMART_PTR<TASK, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > TASK_PTR;

typedef struct SYSTEM {
    ds::LIST<TASK_PTR> taskList; // 0x00 scheduled tasks (owning refs)

    // 0x82783E88 — per-frame tick: start every waiting task, tick every processing task, mark
    // any that became ready as finished (and signal its ready-event), then reap finished tasks.
    void Think();

    // 0x82783??? — attempt to move a waiting task into the processing state (calls CanStart /
    // Execute). boundary — body owned by the gts scheduler drain.
    void TryToStart(TASK_PTR *task);
} SYSTEM;

} // namespace gts
