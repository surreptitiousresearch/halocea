#pragma once

/* real_rgb_color — RGB color, three floats (12 bytes). DB union: flat n[3]
 * view plus the named {red,green,blue} anonymous struct view ($-name kept
 * verbatim; $ in identifiers is an MSVC extension). */

/* DB struct _196A900E1F8FC6D65F3C04EA8B9471C8 — red/green/blue view. */
typedef struct _196A900E1F8FC6D65F3C04EA8B9471C8
{
    float red;   /* 0x00 */
    float green; /* 0x04 */
    float blue;  /* 0x08 */
} _196A900E1F8FC6D65F3C04EA8B9471C8;

typedef union real_rgb_color
{
    float n[3];                                    /* 0x00 */
    struct _196A900E1F8FC6D65F3C04EA8B9471C8 __s1; /* 0x00 — red/green/blue view */
    struct { float red; float green; float blue; };/* 0x00 — direct red/green/blue alias (same layout) */
} real_rgb_color; /* 12 bytes */
