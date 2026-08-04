#pragma once
/* real_euler_angles3d — yaw/pitch/roll triple, radians (12 bytes, math.c).
 * DB union: flat n[3] view plus the named yaw/pitch/roll anonymous struct
 * view ($-name kept verbatim; $ in identifiers is an MSVC extension). */

/* DB struct _026E4EA04987A0F887D0592284F44488 — yaw/pitch/roll view. */
typedef struct _026E4EA04987A0F887D0592284F44488
{
    float yaw;   /* 0x00 */
    float pitch; /* 0x04 */
    float roll;  /* 0x08 */
} _026E4EA04987A0F887D0592284F44488;

typedef union real_euler_angles3d
{
    float n[3];                                    /* 0x00 */
    struct _026E4EA04987A0F887D0592284F44488 __s1; /* 0x00 — yaw/pitch/roll view */
} real_euler_angles3d; /* 12 bytes */
