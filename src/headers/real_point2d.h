#pragma once
/* real_point2d — a 2D point (8 bytes). DB union: flat n[2] view plus x/y and
 * u/v anonymous struct views ($-names kept verbatim; $ in identifiers is an
 * MSVC extension). */

/* DB struct _38C3DEC81229B66F67FB6D350D75FF5A — x/y view. */
typedef struct _38C3DEC81229B66F67FB6D350D75FF5A
{
    float x; /* 0x00 */
    float y; /* 0x04 */
} _38C3DEC81229B66F67FB6D350D75FF5A;

/* DB struct _EF41AC50F64A5069932AD82472589245 — u/v view. */
typedef struct _EF41AC50F64A5069932AD82472589245
{
    float u; /* 0x00 */
    float v; /* 0x04 */
} _EF41AC50F64A5069932AD82472589245;

typedef union real_point2d
{
    float n[2];                                    /* 0x00 */
    struct _38C3DEC81229B66F67FB6D350D75FF5A __s1; /* 0x00 — x/y view */
    struct _EF41AC50F64A5069932AD82472589245 __s2; /* 0x00 — u/v view */
    struct { float x; float y; };                  /* 0x00 — direct x/y alias (same layout) */
    struct { float u; float v; };                  /* 0x00 — direct u/v alias (same layout) */
} real_point2d; /* 8 bytes */
