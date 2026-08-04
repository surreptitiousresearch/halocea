#pragma once
#include "../ds/dsCONST_ARRAY.h"
#include "../ds/dsTSTRING.h"
// ws-engine fsm: a two-point float ramp parsed from a script array.
// DB-verified layout (types_members fsmFLOAT_ARRAY): listValue@0 (dsCONST_ARRAY<float,2>) — size 0x0C.

struct psARRAY; // ps/psARRAY.h — boundary (fwd; used by-reference in ParsePS)

struct fsmFLOAT_ARRAY {
    dsCONST_ARRAY<float, 2> listValue; // 0x00 up to two ramp values

    // 0x831C39C8 (?ParsePS@fsmFLOAT_ARRAY@@QAAXABVpsARRAY@@ABV?$dsTSTRING@D@@@Z) — read the ramp
    // values out of `arr`, tagging parse errors with `hintErr`. boundary (defined elsewhere).
    void ParsePS(const psARRAY &arr, const dsTSTRING<char> &hintErr);

    // ?GetValue@fsmFLOAT_ARRAY@@QBAMXZ — the current ramp value. boundary (defined elsewhere).
    float GetValue() const;
};
