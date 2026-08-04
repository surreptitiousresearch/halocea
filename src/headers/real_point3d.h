#ifndef REAL_POINT3D_H
#define REAL_POINT3D_H

/* real_point3d — 3D point (12 bytes). DB union: flat n[3] view plus two
 * anonymous struct views ($-names kept verbatim; $ in identifiers is an
 * MSVC extension). */

/* DB struct _393C16A032292777F0C3725FFB2C0008 — x/y/z view. */
typedef struct _393C16A032292777F0C3725FFB2C0008
{
    float x; /* 0x00 */
    float y; /* 0x04 */
    float z; /* 0x08 */
} _393C16A032292777F0C3725FFB2C0008;

/* DB struct _465C0AECD0C441C01860649F3839269D — u/v/w view. */
typedef struct _465C0AECD0C441C01860649F3839269D
{
    float u; /* 0x00 */
    float v; /* 0x04 */
    float w; /* 0x08 */
} _465C0AECD0C441C01860649F3839269D;

typedef union real_point3d
{
    float n[3];                                    /* 0x00 */
    struct _393C16A032292777F0C3725FFB2C0008 __s1; /* 0x00 — x/y/z view */
    struct _465C0AECD0C441C01860649F3839269D __s2; /* 0x00 — u/v/w view */
    struct { float x; float y; float z; };         /* 0x00 — direct x/y/z alias (same layout) */
    struct { float u; float v; float w; };         /* 0x00 — direct u/v/w alias (same layout) */
} real_point3d; /* 12 bytes */

#endif /* REAL_POINT3D_H */
