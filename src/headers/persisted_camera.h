#pragma once
/* persisted_camera — one saved editor camera-mode snapshot (32 bytes). persisted_cameras[2]: index 0 is the
 * orbiting camera's saved state, index 1 is the flying camera's saved state (see
 * translate_orbiting_to_flying.c, the only decompiled consumer so far). */

#include "flying_camera.h"

// DB-verified via types_members persisted_camera_data (Blam spelling: persisted_camera)
typedef struct persisted_camera_data
{
    flying_camera camera_data; /* 0x00 */
    unsigned __int8 saved;     /* 0x1C — DB: unsigned __int8 */
    unsigned __int8 _pad1D[3]; /* 0x1D */
} persisted_camera, persisted_camera_data; /* 32 bytes — persisted_camera_data is the DB spelling (used by the storage def) */

extern persisted_camera persisted_cameras[2];
