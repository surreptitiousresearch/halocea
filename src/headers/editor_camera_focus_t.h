#pragma once
/* editor_camera_focus_t — the persistent focus point (position + facing angles) of the free-fly editor
 * camera, seeded from the scenario's first player spawn. */

#include "real_point3d.h"
#include "real_euler_angles2d.h"

/* DB type editor_camera_focus_definition (20 bytes) */
typedef struct editor_camera_focus_definition
{
    real_point3d        position;
    real_euler_angles2d angles;
} editor_camera_focus_definition;

typedef editor_camera_focus_definition editor_camera_focus_t;
