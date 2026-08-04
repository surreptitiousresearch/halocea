#pragma once

/* real_rectangle3d — float 3D rectangle / AABB (24 bytes); indexed view n[6].
 * DB union: flat n[6] view plus the named x0..z1 anonymous struct view
 * ($-name kept verbatim; $ in identifiers is an MSVC extension). */

/* DB struct _3A35C7EA647FFE32EC2151BBF552E106 — x0/x1/y0/y1/z0/z1 view. */
typedef struct _3A35C7EA647FFE32EC2151BBF552E106
{
    float x0; /* 0x00 */
    float x1; /* 0x04 */
    float y0; /* 0x08 */
    float y1; /* 0x0C */
    float z0; /* 0x10 */
    float z1; /* 0x14 */
} _3A35C7EA647FFE32EC2151BBF552E106;

typedef union real_rectangle3d
{
    float n[6];                                    /* 0x00 */
    struct _3A35C7EA647FFE32EC2151BBF552E106 __s1; /* 0x00 — named view */
    struct { float x0; float x1; float y0; float y1; float z0; float z1; }; /* 0x00 — direct alias (same layout) */
} real_rectangle3d; /* 24 bytes */
