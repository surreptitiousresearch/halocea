#pragma once
/* observer_result — a resolved camera/observer viewpoint (observer.c). */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "location.h"

typedef struct observer_result
{
    real_point3d  position;      /* 0x00 */
    location      location;      /* 0x0C — cluster/leaf */
    real_vector3d velocity;      /* 0x14 */
    real_vector3d forward;       /* 0x20 */
    real_vector3d up;            /* 0x2C */
    float         field_of_view; /* 0x38 */
} observer_result;               /* 60 bytes */
