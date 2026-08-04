/* decal_update @0x83740350 — per-tick decal aging: unless the decal is permanent (flags bit 0x2), compute
 * its age in seconds since creation (decal+20). If it isn't due to expire yet (lifetime is infinite, i.e.
 * decal+28 == 0, or age hasn't reached it), start ramping its alpha byte (decal+40) down from 255 to 0
 * over the last decal+32 seconds before expiry. Once it actually expires (age has reached a finite
 * lifetime), unlock it (clearing flags bit 0x1 and decrementing decal_globals->locked_count when it was
 * locked) and delete its render vertices. The alpha byte defaults to -1 (fully opaque) every tick before
 * any of this runs.
 *
 * Decal record fields resolved to the DB decal_datum struct (56-byte stride; see decal_delete.c). */

#include "headers/data_array.h"
#include "headers/decal_globals.h"
#include "headers/decal_datum.h"
#include "headers/decal_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int game_time_get(void);
extern void rasterizer_decal_vertices_delete(int cache_index);

void decal_update(int decal_index)
{
    decal_datum *decal = DATUM_GET(global_decal_data, decal_datum, decal_index);

    unsigned __int16 flags = decal->flags;
    int age_ticks = game_time_get() - decal->creation_time;
    decal->intensity = -1;

    if ( flags & (1u << _decal_permanent_bit) )
        return;

    float lifetime = decal->lifetime;
    float age_seconds = (float)age_ticks * SECONDS_PER_TICK;

    if ( lifetime == 0.0f || age_seconds < lifetime )
    {
        if ( lifetime > 0.0f )
        {
            float fade_duration = decal->decay_time;
            float remaining = lifetime - age_seconds;
            if ( fade_duration > 0.0f && remaining < fade_duration )
                decal->intensity = (int)(remaining / fade_duration * 255.0f);
        }
    }
    else
    {
        if ( flags & (1u << _decal_locked_bit) )
        {
            decal->flags = flags & ~(1u << _decal_locked_bit);
            --decal_globals->locked_count;
        }
        rasterizer_decal_vertices_delete(decal_index);
    }
}
