#pragma once
/* unit_camera_info — resolved camera context for a player's unit: which unit and seat they occupy, the
 * seat's camera definition, and the unit origin. Layout from the database. */

#include <stdint.h>
#include "unit_camera.h"
#include "real_point3d.h"

typedef struct unit_camera_info
{
    int          unit_index;   /* 0x00 — object handle of the occupied unit */
    int16_t      seat_index;   /* 0x04 — seat within the unit (-1 = none) */
    char         _pad6[2];     /* 0x06 */
    unit_camera *unit_camera;  /* 0x08 — seat camera definition (null = default) */
    real_point3d unit_origin;  /* 0x0C */
} unit_camera_info;            /* 24 bytes */
