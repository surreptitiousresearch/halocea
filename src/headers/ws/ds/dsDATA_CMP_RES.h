#pragma once
// ws-engine ds_data: three-way comparison result returned by dsDATA_TYPE::Compare paths.
// DB-verified (types_enum_values dsDATA_CMP_RES) — size 4.

enum dsDATA_CMP_RES {
    DSD_CMP_UNDEF   = -1,
    DSD_CMP_EQUAL   = 0,
    DSD_CMP_GREATER = 1,
    DSD_CMP_LESS    = 2,
};
