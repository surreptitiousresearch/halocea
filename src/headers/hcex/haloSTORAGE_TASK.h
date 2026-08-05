#pragma once
#include "../ws/sys/sysUSER_STORAGE_TASK.h"
// hcex bridge: HCEX's concrete sysUSER_STORAGE_TASK for save/load requests routed through the
// ws-engine sysUSER storage API. DB-verified layout (types_members haloSTORAGE_TASK):
// sysUSER_STORAGE_TASK (base)@0 (156 bytes), error@156 (int), isReady@160 (bool) -- size ~161
// (padded).

struct haloSTORAGE_TASK : sysUSER_STORAGE_TASK {
    int  error;     // 0x9C (156) -- GetErrorCode's return; 0 = no error
    bool isReady;   // 0xA0 (160) -- set by both Execute and Cancel; read by IsReady/IsSuccess

    // 0x823F4C14 — forwards to the sysUSER_STORAGE_TASK base ctor (re-acquiring a reference on
    // `u`'s underlying REF_TYPE_DATA around the call, matching the compiler's by-value-param
    // refcount convention), then resets error/isReady to 0.
    haloSTORAGE_TASK(ds_data::REF_TYPE<sysUSER> u, TASK_TYPE t, sysUSER_CONTENT_TYPE ct, bool async);

    // 0x823F4984 — no owned resources of its own; just chains to ~sysUSER_STORAGE_TASK.
    ~haloSTORAGE_TASK();

    // 0x823F4A14 — GetErrorCode: return the stored error code. Public virtual (UAAHXZ), overrides
    // sysUSER_STORAGE_TASK's GetErrorCode.
    int GetErrorCode();

    // 0x823F49BC — IsReady: return whether Execute/Cancel has completed. Public virtual (UAA_NXZ).
    bool IsReady();

    // 0x823F4A00 — IsSuccess: ready AND no error. Public virtual (UAA_NXZ).
    bool IsSuccess();

    // 0x823F4A24 — Execute: mark ready (the actual async I/O is presumably driven by the
    // scheduler elsewhere / not observed at this call site). Protected virtual (MAAXXZ).
    void Execute();

    // 0x823F4A44 — Cancel: mark ready without ever setting an error. Protected virtual (MAAXXZ).
    void Cancel();

    // 0x823F4A30 — Think: no per-tick work for this task type. Protected virtual (MAAXXZ).
    void Think();
};
