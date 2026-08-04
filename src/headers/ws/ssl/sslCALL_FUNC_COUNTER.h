#pragma once
// ssl subsystem: RAII guard scoping an in-progress native script-callback invocation on its owning
// sslSYSTEM (bumps the system's active-call bookkeeping on construct, restores it on destroy).
// DB-verified layout (types_members sslCALL_FUNC_COUNTER): Ssl@0 (sslSYSTEM*) — size 4.

struct sslSYSTEM; // boundary — owning script system

typedef struct sslCALL_FUNC_COUNTER {
    sslSYSTEM *Ssl; // 0x00 owning system

    // Nesting depth of active native-callback invocations, shared across every instance
    // (DB symbols sslCALL_FUNC_COUNTER::Count / ::Skip).
    static int Count;
    static int Skip;

    // 0x83087B68 — bind `ssl`, bump the process-wide nesting counter, and arm the debugger
    // single-step trap (dbgMode 2->3, recording Skip = the post-increment Count) when the owning
    // system is paused at a breakpoint about to step into this call.
    explicit sslCALL_FUNC_COUNTER(sslSYSTEM *ssl);
    // 0x83087BA8 — undo the debugger single-step arm/latch (dbgMode 3->2 on the matching depth,
    // or 4->2 unconditionally), then on the outermost unwind (Count reaches 0) leave dbgMode
    // paused (2) and notify the console debugger UI ("Ssl"/"SSL_Dlg"/"StopTrace") if connected.
    ~sslCALL_FUNC_COUNTER();
} sslCALL_FUNC_COUNTER;
