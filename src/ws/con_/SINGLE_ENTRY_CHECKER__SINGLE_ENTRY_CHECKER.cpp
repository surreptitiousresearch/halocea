#include "SINGLE_ENTRY_CHECKER.h"
#include "../ds/ds_assert_boundary.h"

// os-layer atomic primitives (osLOCK.h covers the osLOCK struct but not these free
// functions) -- boundary.
extern "C" int osLockedCompareAndSwap(int *slot, int exchange, int compare); // boundary (def order: destination, exchange, comperand)
extern "C" int osLockedAdd(int *slot, int delta);                           // boundary

// con::SINGLE_ENTRY_CHECKER::SINGLE_ENTRY_CHECKER(int&) @ 0x82AC72E0
// Reentrancy-guard constructor: asserts the shared counter is currently 0 (not
// already "entered"), then atomically increments it. The paired destructor
// (not part of this batch) presumably decrements it back.
con::SINGLE_ENTRY_CHECKER::SINGLE_ENTRY_CHECKER(int &nEntry_)
{
    this->nEntry = &nEntry_;
    if (!IGNORE_STRONG_ASSERT && osLockedCompareAndSwap(&nEntry_, 0, 0))
        STRONG_ASSERT_DUMMY().Crash(
            "osLockedRead(&nEntry) == 0",
            "D:\\Projects\\code\\common\\src.sys\\console\\conComm.cpp",
            307,
            dsStrongAssertMessage);
    osLockedAdd(this->nEntry, 1);
}
