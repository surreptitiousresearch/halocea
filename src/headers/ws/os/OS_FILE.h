#pragma once
// ws-engine os: bitflag enum for osFileOpen's mode parameter (wrapped by dsFLAGS<OS_FILE,int>).
// DB-verified (types_enum_values OS_FILE): all 8 values, each a distinct bit.
enum OS_FILE {
    OS_FILE_READ         = 1,
    OS_FILE_WRITE        = 2,
    OS_FILE_APPEND       = 4,
    OS_FILE_CREATE       = 8,
    OS_FILE_TRUNCATE     = 16,
    OS_FILE_MAKEDIR      = 32,
    OS_FILE_CHECK_DIRTY  = 64,
    OS_FILE_NO_CACHE     = 128,
};
