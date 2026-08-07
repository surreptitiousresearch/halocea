#pragma once
// ws-engine anim instance — BOUNDARY alias header.
//
// This file used to carry a SECOND file-scope body for `animINST`: a partial padding model exposing
// only name@0x28 / pPhysSystem@0xB4 / pObj@0x148. The full DB-verified 536-byte layout
// (types_members animINST, DB size 536) already lives in src/headers/animINST.h, so which layout a
// TU saw depended on include order. Forwarded to the canonical header instead. The include is
// PATH-QUALIFIED on purpose: `animINST.h` is also the basename of ws/anim/animINST.h (itself a
// forwarder), so a bare `#include "animINST.h"` would resolve by include-directory order.
//
// Nothing correct was lost in the merge — both claims the deleted body carried are refuted by the
// binary, and both are already spelled correctly in animINST.h:
//   * `void GetPos(m3dV *) const` — the DB mangle is ?GetPos@animINST@@QAAXPAUm3dV@@@Z
//     (QAA = public NON-const; QBA would be const).
//   * `GetOBB(int boneIndex)`, "0 = whole instance" — the disasm at 0x825E4E30 tests the argument
//     only to SKIP animINST::Validate(0x100), then returns `this + 0xC4` (the one `obb` member)
//     on every path. The parameter is forceNoValidate, as animINST.h states; it is not a bone index.
#include "../../animINST.h"
