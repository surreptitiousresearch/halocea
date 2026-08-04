#pragma once
// navWFUNC_SQAS — a nav weighting function (squared-distance / assoc) used by aiCOORDINATOR's
// used-WP penalty math (aiCOORDINATOR::CalcPenaltyFromUsedWPs dispatches GetWeight_D2). The full
// DB-verified layout and the GetWeight_D2 declaration are canonical in navWFUNC_DIST.h; this
// boundary header just re-exports it so existing consumers keep compiling.
#include "navWFUNC_DIST.h"
