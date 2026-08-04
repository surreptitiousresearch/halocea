#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* widget_occlusion_vertex — a single vertex of the lens-flare occlusion-test quad (28 bytes):
 * transformed screen position, packed diffuse + specular colors, and a texture coordinate. */

#include "real_point3d.h"
#include "real_point2d.h"

typedef struct widget_occlusion_vertex
{
    real_point3d position;    /* 0x00 */
    unsigned int diffuse;     /* 0x0C */
    unsigned int specular;    /* 0x10 */
    real_point2d texcoord;    /* 0x14 */
} widget_occlusion_vertex;    /* 0x1C */
