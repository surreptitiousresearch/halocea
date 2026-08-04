#pragma once
// ws-engine vid: per-adapter shader-platform capability flags. DB-verified layout
// (types_members vidPASS_PLATFORM, size 7) and nested enum values (types_enum_values
// vidPASS_PLATFORM::ID / ::VENDOR).

typedef struct vidPASS_PLATFORM
{
    enum ID     { ID_PC = 0, ID_XBOX360 = 1, ID_PS3 = 2 };
    enum VENDOR { VENDOR_ATI = 0, VENDOR_NV = 1 };

    ID     id;             /* 0x0 (1 byte) */
    VENDOR vendor;         /* 0x1 (1 byte) */
    bool   dxn;            /* 0x2 DXN-compressed-normal-map hardware support */
    bool   tess_support;   /* 0x3 */
    bool   z_sample_dot;   /* 0x4 */
    bool   z_sample_float; /* 0x5 */
    bool   fetch4;         /* 0x6 */
} vidPASS_PLATFORM;
