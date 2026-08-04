/* director_dispose_from_old_map @ 0x836E44A0-adjacent (0x836E4C40) — reset both local
 * players' director (camera) state. */
#include "headers/director.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler modeled a 9-byte director {debug_input_scale@0, camera_proc@4,
 * debug_controls@8}; the DB-verified layout (director.h) is 248 bytes with camera_proc@8,
 * debug_controls@0xC0, debug_input_scale@0xC4 and local_players@8 — disasm confirms stride 0xF8 and those
 * offsets. Reusing the canonical header corrects the offsets and array stride. */
void director_dispose_from_old_map(void)
{
    int i;
    for ( i = 0; i < 2; ++i )
    {
        director *d = &director_globals.local_players[i];
        d->debug_input_scale = 1.0f;
        d->camera_proc = 0;
        d->debug_controls = 0;
    }
    *director_camera_scripted = 0;
}
