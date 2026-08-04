#pragma once
// ws-engine con: scope-guard reentrancy checker. Construction asserts the shared
// counter is currently zero, then atomically increments it; used to catch
// reentrant calls into non-reentrant console code paths.
// NOTE: this subsystem's prefix is "con", but the directory is "con_" because "con" is a
// Windows-reserved device name that git/Win32 cannot index.
// DB-verified layout (types_members con::SINGLE_ENTRY_CHECKER) -- size 4.

namespace con {

typedef struct SINGLE_ENTRY_CHECKER {
    int *nEntry; // 0x00 shared reentrancy counter (int&, passed by address per ABI)

    SINGLE_ENTRY_CHECKER(int &nEntry_); // 0x82AC72E0
} SINGLE_ENTRY_CHECKER;

} // namespace con
