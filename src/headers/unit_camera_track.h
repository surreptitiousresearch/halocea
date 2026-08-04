#pragma once
/* unit_camera_track — a seat/unit camera-track reference (28 bytes). DB layout.
 * track.index (tag_reference +0x0C) names the camera_track_definition animation tag. */

#include "tag_reference.h"

typedef struct unit_camera_track
{
    tag_reference track;      /* 0x00 (index @0x0C) */
    int           unused[3];  /* 0x10 */
} unit_camera_track;          /* 28 bytes */
