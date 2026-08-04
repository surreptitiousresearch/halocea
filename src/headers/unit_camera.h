#pragma once
/* unit_camera — the camera definition for a unit/vehicle seat: which marker the eye rides, plus pitch
 * autolevel target and pitch limits (radians). Layout from the database. */

#include "tag_block.h"

typedef struct unit_camera
{
    char      marker_name[32];           /* 0x00 */
    char      submerged_marker_name[32]; /* 0x20 */
    float     pitch_autolevel;           /* 0x40 — target pitch the camera eases toward (0 = none) */
    float     pitch_minimum;             /* 0x44 */
    float     pitch_maximum;             /* 0x48 */
    tag_block unit_camera_tracks;        /* 0x4C */
} unit_camera;                           /* 88 bytes */
