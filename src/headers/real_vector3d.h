#ifndef REAL_VECTOR3D_H
#define REAL_VECTOR3D_H

/* real_vector3d — 3-component float vector (12 bytes). DB union: flat n[3]
 * view plus the anonymous i/j/k struct view ($-name kept verbatim; $ in
 * identifiers is an MSVC extension). */

/* DB struct _B0BFFDEFFA2505C29E87DE0486E0E59E — i/j/k view. */
typedef struct _B0BFFDEFFA2505C29E87DE0486E0E59E
{
    float i; /* 0x00 */
    float j; /* 0x04 */
    float k; /* 0x08 */
} _B0BFFDEFFA2505C29E87DE0486E0E59E;

typedef union real_vector3d
{
    float n[3];                                    /* 0x00 */
    struct _B0BFFDEFFA2505C29E87DE0486E0E59E __s1; /* 0x00 — i/j/k view */
    struct { float i; float j; float k; };         /* 0x00 — direct i/j/k alias (same layout) */
} real_vector3d; /* 12 bytes */

#endif /* REAL_VECTOR3D_H */
