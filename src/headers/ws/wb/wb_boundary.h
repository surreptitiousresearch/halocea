#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "dbgVARWrapper.h"
// ws-engine wb (watch/debug window): namespace-scope state and sibling helpers that this
// batch's targets (IsActive/Printf/RegisterPage/dbgVARWrapper::GetValue) call but do not
// themselves reverse -- boundary, per the shared re-source brief's "un-reversed same-
// namespace calls stay boundary" rule.

namespace wb {

// One text buffer per registered debug page (its accumulated Printf output for this frame).
extern dsVECTOR<dsTSTRING<char>, 8> pages;
// One dbgVARWrapper per registered page -- the "is this page active" bool debug variable.
extern dsVECTOR<wb::dbgVARWrapper, 8> vars;
// Per-page flags bitmask, as passed to RegisterPage.
extern dsVECTOR<int, 8> pageFlags;

// wb::GetMaxPage() @ 0x8269DC68 — pages.nElem. boundary.
int GetMaxPage();
// wb::GetPage(int) @ 0x8269E050 — bounds-checked pages[idx] (asserts, falls back to pages.pData
// on an out-of-range index). boundary.
dsTSTRING<char> *GetPage(int idx);
// wb::GetVarValue(int) @ 0x8269E108 — bounds-checked vars[idx].GetValue() (asserts + crashes,
// returns false on an out-of-range index). boundary.
bool GetVarValue(int idx);
// wb::GetPageFlags(int) @ 0x8269E198 — bounds-checked pageFlags[idx]. boundary.
int GetPageFlags(int idx);

} // namespace wb
