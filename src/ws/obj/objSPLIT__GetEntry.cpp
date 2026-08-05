#include "objSPLIT.h"
#include "../ds/ds_assert_boundary.h"
// 0x82681828  ?GetEntry@objSPLIT@@QAAPAVobjSPLIT_ENTRY@@H@Z
// Bounds-checked access to split entry `i`.
//
// CAVEAT: the assert-disabled fallback clamps a negative index to 0 (and otherwise passes the
// index through) via the branchless `(((unsigned)i >> 31) - 1) & i`; reproduced faithfully.

objSPLIT_ENTRY *objSPLIT::GetEntry(int i)
{
    if (!IGNORE_STRONG_ASSERT && (i < 0 || i >= this->nEntry))
        STRONG_ASSERT_DUMMY().Crash(
            "i >= 0 && i < nEntry",
            "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 512,
            empty_string);

    int entryIndex = this->nEntry;
    if (i <= entryIndex)
        entryIndex = (((unsigned int)i >> 31) - 1) & i; // clamp negative index to 0
    return &this->entryList[entryIndex];
}
