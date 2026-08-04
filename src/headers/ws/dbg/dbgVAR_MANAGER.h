#pragma once
#include "../con_/CONSOLE_CALLBACK.h"
#include "../ds/dsVECTOR.h"
#include "../ds/MAP.h"
#include "../os/osLOCK.h"
#include "dbgVAR_boundary.h"
// ws-engine dbg: the process-wide registry of debug variables (dbgVAR instances). Publicly
// inherits con::CONSOLE_CALLBACK so it can register itself as the console's "vars" debug page.
// DB-verified layout (types_members dbgVAR_MANAGER):
//   con::CONSOLE_CALLBACK @0 (size 16, anonymous base -- promoted via inheritance)
//   vars@16 (dsVECTOR<dbgVAR*,8>, size 20), newVars@36 (dsVECTOR<dbgVAR*,8>, size 20),
//   needUpdateConsole@56 (bool), lock@60 (osLOCK, size 52),
//   overrides@112 (ds::MAP<dsTSTRING<char>,dsTSTRING<char>,ds::HASH,ds::CMP,
//   dsNODE_CACHE_ALLOCATOR>, size 84) -- total size 196.

struct dbgVAR_MANAGER : con::CONSOLE_CALLBACK {
    dsVECTOR<dbgVAR *, 8> vars;               // 0x10 every registered debug variable
    dsVECTOR<dbgVAR *, 8> newVars;            // 0x24 variables added since the console page last synced
    bool                   needUpdateConsole; // 0x38 set when vars/newVars changed and the console page needs a refresh
    unsigned char          _pad39[3];         // 0x39
    osLOCK                 lock;              // 0x3C guards vars/newVars/needUpdateConsole
    // 0x70 name -> override-value map (e.g. loaded from a debug-var override config). boundary usage.
    ds::MAP<dsTSTRING<char>, dsTSTRING<char>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> overrides;

    // ??0dbgVAR_MANAGER@@QAA@XZ @ 0x82560e00 -- default constructor: default-constructs the base
    // CONSOLE_CALLBACK, resets vars/newVars to empty (tagged with this ctor's call-site cookie),
    // clears needUpdateConsole, hand-initialises then Init()s `lock`, and default-constructs
    // `overrides` (same call-site cookie). Load-bearing for GetManager's static local.
    dbgVAR_MANAGER();

    // ?GetManager@dbgVAR_MANAGER@@SAPAV1@XZ @ 0x82561210 -- process-wide singleton accessor.
    // Reconstructed as an idiomatic C++ function-local static (the binary's guard-bit '_S1_12' +
    // atexit-registered dynamic destructor for the local `m` is exactly what a Meyers singleton
    // compiles to).
    static dbgVAR_MANAGER *GetManager();

    // ?AddVar@dbgVAR_MANAGER@@QAAXPAVdbgVAR@@@Z @ 0x82560488 -- register a newly-constructed debug
    // variable: append it to both `vars` and `newVars` (so the console page picks it up on its next
    // sync) and flag needUpdateConsole, all under `lock`.
    void AddVar(dbgVAR *var);

    // ?FindFirst@dbgVAR_MANAGER@@QAAPAVdbgVAR@@ABV?$dsTSTRING@D@@@Z @ 0x8255FB00 -- linear scan of
    // `vars` under `lock` for the first entry whose name exactly equals `name` (shared-buffer
    // pointer-equality is a fast path; otherwise a length-then-byte compare). Returns null if
    // absent.
    dbgVAR *FindFirst(const dsTSTRING<char> &name);

    // ??$SetDbgVar@V?$dsTSTRING@D@@@dbgVAR_MANAGER@@QAAXABV?$dsTSTRING@D@@0@Z -- member template,
    // parameterised on the VALUE type T and its dbgVAR typeId N (matches dbgVAR_IMPL<T,N>). Looks
    // up the dbgVar named `name`; if found and its typeId equals N, forwards to SetDbgVarValue to
    // overwrite its current value with `val`. e.g. <dsTSTRING<char>,4> @ 0x823b54b8.
    template<class T, int N>
    void SetDbgVar(const dsTSTRING<char> &name, const T &val);

protected:
    // ??$SetDbgVarValue@V?$dsTSTRING@D@@@dbgVAR_MANAGER@@AAAXPAVdbgVAR@@ABV?$dsTSTRING@D@@@Z --
    // member template (AAA mangle = protected), parameterised the same way as SetDbgVar. Overwrites
    // `var`'s current value field (a dbgVAR_IMPL<T,N>, reached via a static_cast on `var`) by
    // sharing `val`'s buffer/representation, then calls SendVarUpdate. e.g. <dsTSTRING<char>,4>
    // @ 0x823b51b0.
    template<class T, int N>
    void SetDbgVarValue(dbgVAR *var, const T &val);

private:
    // ?SendVarUpdate@dbgVAR_MANAGER@@AAAXPAVdbgVAR@@@Z @ 0x8255E020 -- notify the debug console's
    // "vars" page that `var`'s value changed. Private (AAA mangle).
    void SendVarUpdate(dbgVAR *var);
    // HCEX boundary flat-bridge shim (dbgVAR_MANAGER_flat_bridge.cpp) forwards the flattened
    // spelling haloInit_boundary.h declares into the private member above.
    friend void dbgVAR_MANAGER_SendVarUpdate(dbgVAR_MANAGER *mgr, dbgVAR *var);
};
