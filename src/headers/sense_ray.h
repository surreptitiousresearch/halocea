#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* sense_ray — an actor perception/avoidance probe: a ray of given length cast from an offset with angular
 * divergence. */

#include "real_vector3d.h"

typedef struct sense_ray
{
    float        length;     /* 0x00 */
    real_vector3d offset;     /* 0x04 */
    real_vector3d divergence; /* 0x10 */
} sense_ray;                  /* 28 bytes */
