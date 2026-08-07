#pragma once
// ws-engine mdl: mdlITRC_BASE::stateUser flag bits. DB-verified (types_enum_values ITRC_ST); size 4.
// ITRCST_LASTSTATE is the binary's own sentinel (highest bit | 1), transcribed verbatim.
enum ITRC_ST {
    ITRCST_HO_MELEE               = 0x1,
    ITRCST_HO_MELEE_ALLOW_UPDATE  = 0x2,
    ITRCST_FREE_MELEE             = 0x4,
    ITRCST_SKIP_MELEE             = 0x8,
    ITRCST_HO_FREEZE_CUSTOM_LOGIC = 0x10,
    ITRCST_LASTSTATE              = 0x11,
};
