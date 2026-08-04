#pragma once
/* camera_track_definition — a unit camera-track animation tag (48 bytes). DB layout.
 * control_points is the keyframe array (camera_track_control_point, 60 bytes each). */

#include "tag_block.h"

typedef struct camera_track_definition
{
    unsigned int flags;          /* 0x00 */
    tag_block    control_points;  /* 0x04 (count @0x04, address @0x08) */
    int          unused[8];       /* 0x10 */
} camera_track_definition;        /* 48 bytes */
