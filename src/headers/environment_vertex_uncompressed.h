#pragma once
/* environment_vertex_uncompressed — the decoded structure render vertex: position plus a full
 * float basis (normal/binormal/tangent) and a diffuse texcoord. Layout confirmed against the
 * database type of the same name (56 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_point2d.h"

typedef struct environment_vertex_uncompressed
{
    real_point3d  position;    /* 0x00 */
    real_vector3d normal;      /* 0x0C */
    real_vector3d binormal;    /* 0x18 */
    real_vector3d tangent;     /* 0x24 */
    real_point2d  texcoord;    /* 0x30 */
} environment_vertex_uncompressed;   /* 0x38 = 56 bytes */
