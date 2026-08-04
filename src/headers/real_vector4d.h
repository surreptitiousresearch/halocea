#pragma once
/* real_vector4d — 4-component float vector (16 bytes). DB union: flat n[4]
 * view plus the anonymous i/j/k/l struct view ($-name kept verbatim; $ in
 * identifiers is an MSVC extension). */

/* DB struct _A32E696702694F14A2D5A1B23FA4E934 — i/j/k/l view. */
typedef struct _A32E696702694F14A2D5A1B23FA4E934
{
    float i; /* 0x00 */
    float j; /* 0x04 */
    float k; /* 0x08 */
    float l; /* 0x0C */
} _A32E696702694F14A2D5A1B23FA4E934;

typedef union real_vector4d
{
    float n[4];                                    /* 0x00 */
    struct _A32E696702694F14A2D5A1B23FA4E934 __s1; /* 0x00 — i/j/k/l view */
} real_vector4d; /* 16 bytes */
