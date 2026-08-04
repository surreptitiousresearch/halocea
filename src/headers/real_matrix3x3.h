#pragma once

#include "real_vector3d.h"

/* real_matrix3x3 — pure 3x3 rotation matrix (36 bytes, no scale/position
 * row). DB: struct with a single anonymous-union member ___u0 uniting a
 * plain n[row][col] float-array view and named row fields (forward/left/up).
 * ($-names kept verbatim; $ in identifiers is an MSVC extension.) */

/* DB struct _6D4B49AA0938E380AAA62D5657D0A726 — named row view. */
typedef struct _6D4B49AA0938E380AAA62D5657D0A726
{
    real_vector3d forward; /* 0x00 */
    real_vector3d left;    /* 0x0C */
    real_vector3d up;      /* 0x18 */
} _6D4B49AA0938E380AAA62D5657D0A726;

/* DB union _F5BC8CA832E91F612524809E7CBFA9D7 — row views. */
typedef union _F5BC8CA832E91F612524809E7CBFA9D7
{
    float n[3][3];                                 /* 0x00 */
    struct _6D4B49AA0938E380AAA62D5657D0A726 __s1; /* 0x00 — forward/left/up view */
} _F5BC8CA832E91F612524809E7CBFA9D7;

typedef struct real_matrix3x3
{
    /* Anonymous outer union exposes the DB named ___u0 view AND direct
     * n[]/forward/left/up views at offset 0; both access styles compile.
     * Layout unchanged (36 bytes). */
    union {
        union _F5BC8CA832E91F612524809E7CBFA9D7 ___u0; /* 0x00 — DB named view */
        float n[3][3];                                 /* 0x00 — direct array view */
        struct {
            real_vector3d forward; /* 0x00 */
            real_vector3d left;    /* 0x0C */
            real_vector3d up;      /* 0x18 */
        };
    };
} real_matrix3x3; /* 36 bytes */
