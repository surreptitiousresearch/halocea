#pragma once
/* real_euler_angles2d — yaw/pitch pair, radians (8 bytes). DB union: flat
 * n[2] view plus the named yaw/pitch anonymous struct view ($-name kept
 * verbatim; $ in identifiers is an MSVC extension). */

/* DB struct _C5FEA69552C5FE5632EF6BDA47F759D9 — yaw/pitch view. */
typedef struct _C5FEA69552C5FE5632EF6BDA47F759D9
{
    float yaw;   /* 0x00 */
    float pitch; /* 0x04 */
} _C5FEA69552C5FE5632EF6BDA47F759D9;

typedef union real_euler_angles2d
{
    float n[2];                                    /* 0x00 — n[0]=yaw, n[1]=pitch */
    struct _C5FEA69552C5FE5632EF6BDA47F759D9 __s1; /* 0x00 — yaw/pitch view */
    struct { float yaw; float pitch; };            /* 0x00 — direct yaw/pitch alias (same layout) */
} real_euler_angles2d; /* 8 bytes */
