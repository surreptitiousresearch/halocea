#pragma once
#include "../ds/dsFIXED_BIT_ARRAY.h"
// ws-engine vid/sdr: full shader-state bit set of a render pass. DB-verified (types_members
// SDR_FULL_STATES): a single dsFIXED_BIT_ARRAY<6> base at offset 0 (size 24, no added data).

struct SDR_FULL_STATES : dsFIXED_BIT_ARRAY<6> {
};
