#pragma once
#include "../ds/dsREF_COUNT.h"
#include "../ds/dsTSTRING.h"
// ws-engine gts (game task scheduler): base class for a scheduled/async unit of work.
// BOUNDARY -- only the layout needed to type sysUSER_STORAGE_TASK's base is modeled here; the
// task-scheduling machinery itself is external to this batch.
// DB-verified layout (types_members gts::TASK): __vftable@0, dsREF_COUNT (base)@4, id@8 (int),
// state@12 (gts::TASK_STATE) -- size 16. The vtable pointer sits before the dsREF_COUNT base
// because TASK is the first class in this chain to introduce virtual functions while its base
// is non-polymorphic (standard MSVC layout for that pattern).

namespace gts {

enum TASK_STATE : int { // DB types_enum_values gts::TASK_STATE
    TS_INITED     = 0,
    TS_WAITING    = 1,
    TS_PROCESSING = 2,
    TS_FINISHED   = 3,
};

struct TASK;

typedef struct TASK_vtbl { // DB types_members gts::TASK_vtbl
    void (*dtr_TASK)(TASK *self, int deleteFlag);                    // 0x00 (scalar-deleting dtr; deleteFlag bit0 => free)
    int  (*GetReadyEventID)(TASK *self);                             // 0x04
    bool (*IsReady)(TASK *self);                                     // 0x08
    bool (*IsSuccess)(TASK *self);                                   // 0x0C
    int  (*GetErrorCode)(TASK *self);                                // 0x10
    dsTSTRING<char> *(*GetErrorDbgStr)(TASK *self, dsTSTRING<char> *result); // 0x14
    bool (*CanStart)(TASK *self);                                    // 0x18
    void (*Execute)(TASK *self);                                     // 0x1C
    void (*Cancel)(TASK *self);                                      // 0x20
    void (*Think)(TASK *self);                                       // 0x24
} TASK_vtbl;

typedef struct TASK {
    TASK_vtbl  *__vftable; // 0x00
    dsREF_COUNT refCount;  // 0x04 <base> dsREF_COUNT (vtable-first MSVC layout)
    int         id;        // 0x08
    TASK_STATE  state;     // 0x0C
} TASK;

} // namespace gts
