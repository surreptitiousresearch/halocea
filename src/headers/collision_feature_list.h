/* collision_feature_list — features (spheres/cylinders/prisms) gathered within a query sphere by
 * collision_get_features_in_sphere, then point/vector-tested. Layout verbatim from the database. */
#pragma once

#include "collision_sphere.h"
#include "collision_cylinder.h"
#include "collision_prism.h"

typedef struct collision_feature_list
{
    __int16            count[3];        /* 0x0000 — {sphere_count, cylinder_count, prism_count} */
    unsigned char _pad0[2]; /* db-verified padding */
    collision_sphere   spheres[256];    /* 0x0008 */
    collision_cylinder cylinders[256];  /* 0x1C08 */
    collision_prism    prisms[256];     /* 0x4408 */
} collision_feature_list;
