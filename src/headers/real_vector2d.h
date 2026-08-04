#pragma once

/* real_vector2d — 2-component float vector (8 bytes). DB union: flat n[2]
 * view plus the anonymous i/j struct view ($-name kept verbatim; $ in
 * identifiers is an MSVC extension). */

/* DB struct _F790B8067189E776CFBEFB7C6A6E34F1 — i/j view. */
typedef struct _F790B8067189E776CFBEFB7C6A6E34F1
{
    float i; /* 0x00 */
    float j; /* 0x04 */
} _F790B8067189E776CFBEFB7C6A6E34F1;

typedef union real_vector2d
{
    float n[2];                                    /* 0x00 */
    struct _F790B8067189E776CFBEFB7C6A6E34F1 __s1; /* 0x00 — i/j view */
    struct { float i; float j; };                  /* 0x00 — direct i/j alias (same layout) */
} real_vector2d; /* 8 bytes */
