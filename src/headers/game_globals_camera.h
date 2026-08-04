#pragma once
/* game_globals_camera — the camera block of the game_globals ('matg') tag (16 bytes, DB-verified). */

#include "tag_reference.h"

typedef struct game_globals_camera
{
    tag_reference default_unit_camera_track;  /* 0x00 */
} game_globals_camera;                         /* 16 bytes */
