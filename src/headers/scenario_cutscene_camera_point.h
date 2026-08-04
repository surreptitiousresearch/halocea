#ifndef SCENARIO_CUTSCENE_CAMERA_POINT_H
#define SCENARIO_CUTSCENE_CAMERA_POINT_H

/* scenario_cutscene_camera_point — one element of scenario.cutscene_camera_points (104-byte stride).
 * Layout from the IDA DB type `scenario_cutscene_camera_point` (types_members, ordinal 41609). */

#include "real_point3d.h"
#include "real_euler_angles3d.h"

typedef struct scenario_cutscene_camera_point
{
    int                 flags;          /* 0x00 */
    char                name[32];       /* 0x04 */
    int                 pad;            /* 0x24 */
    real_point3d        position;       /* 0x28 */
    real_euler_angles3d orientation;    /* 0x34 */
    float               field_of_view;  /* 0x40 */
    int                 unused[9];      /* 0x44 */
} scenario_cutscene_camera_point;       /* 104 bytes */

#endif /* SCENARIO_CUTSCENE_CAMERA_POINT_H */
