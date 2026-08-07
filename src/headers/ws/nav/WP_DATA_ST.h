#pragma once
// ws-engine nav: navWP_DATA state flag bits. DB-verified (types_enum_values WP_DATA_ST); size 4.
// WPDT_LASTSTATE is the binary's own sentinel (highest bit | 1), transcribed verbatim.
enum WP_DATA_ST {
    WPDT_NONE                     = 0,
    WPDT_NON_SKIPABLE             = 0x1,
    WPDT_COUNTER_WAS_APPLIED      = 0x2,
    WPDT_COUNTER_WAS_TMP_REMOVED  = 0x4,
    WPDT_LASTSTATE                = 0x5,
};
