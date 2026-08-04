#include "headers/blam_data_globals.h"
/* director_initialize @ 0x836E44A0 — allocate the camera-scripted flag (4 bytes) */

extern void *game_state_malloc(const char *name, const char *type, int size);

/* void: caller game_initialize ignores r3; the r3 at blr is only game_state_malloc's
 * residue (reused as the *ptr=0 store base), no independent return value. */
void director_initialize(void)
{
    director_camera_scripted = game_state_malloc("director scripting", 0, 4);
    *director_camera_scripted = 0;
}
