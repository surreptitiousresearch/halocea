#pragma once

#include "real_vector3d.h"
#include "real_point3d.h"

/* real_matrix4x3 — affine 4x3 matrix: uniform scale + 4 rows of 3 (52
 * bytes). DB: struct {scale; ___u1} where ___u1 unites a plain n[row][col]
 * float-array view and named row fields (forward/left/up/position).
 * ($-names kept verbatim; $ in identifiers is an MSVC extension.) */

/* DB struct _E5445413AE77EB593AC1569CACA0ADB2 — named row view. */
typedef struct _E5445413AE77EB593AC1569CACA0ADB2
{
    real_vector3d forward;  /* 0x00 */
    real_vector3d left;     /* 0x0C */
    real_vector3d up;       /* 0x18 */
    real_point3d  position; /* 0x24 */
} _E5445413AE77EB593AC1569CACA0ADB2;

/* DB union _8720DA82371E61A926AF87C1ED38CB66 — row views. */
typedef union _8720DA82371E61A926AF87C1ED38CB66
{
    float n[4][3];                                 /* 0x00 */
    struct _E5445413AE77EB593AC1569CACA0ADB2 __s1; /* 0x00 — named row view */
} _8720DA82371E61A926AF87C1ED38CB66;

typedef struct real_matrix4x3
{
    float scale;                                   /* 0x00 */
    /* Anonymous outer union exposes the DB's named ___u1 view AND direct
     * n[]/forward/... views at offset 0x04; all consumers (both matrix.___u1.n
     * and matrix.n / matrix.forward styles) compile. Layout unchanged (52B). */
    union {
        union _8720DA82371E61A926AF87C1ED38CB66 ___u1; /* 0x04 — DB named view */
        float n[4][3];                                 /* 0x04 — direct array view */
        struct _E5445413AE77EB593AC1569CACA0ADB2 __s1_direct; /* 0x04 — direct named-row view */
        struct {
            real_vector3d forward;  /* 0x04 */
            real_vector3d left;     /* 0x10 */
            real_vector3d up;       /* 0x1C */
            real_point3d  position; /* 0x28 */
        };
    };
} real_matrix4x3; /* 52 bytes */
