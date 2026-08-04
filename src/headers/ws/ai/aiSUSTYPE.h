#pragma once
// ws-engine ai08 — "suspicion/attention" point classification.
// DB-verified enum values (types_enum_values aiSUSTYPE).

enum aiSUSTYPE {
    ASUT_NA             = -1,
    ASUT_RANDOM         = 0,
    ASUT_REAL_PLR       = 1,
    ASUT_REAL_PLR2      = 2,
    ASUT_LAST_CORPSE    = 3,
    ASUT_LAST_SHOT      = 4,
    ASUT_LAST_SEEN_PLR  = 5,
    ASUT_LAST_SEEN_PLR2 = 6,
    ASUT_MAIN           = 7,
    ASUT_MAIN2          = 8,
    ASUT_LAST           = 9,
};
