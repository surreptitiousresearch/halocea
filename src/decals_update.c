/* decals_update @0x83740664 — per-frame decal aging. For every live, non-permanent decal: computes
 * its age in seconds, and either fades its alpha as it approaches the end of its lifetime, or (once
 * expired) unlocks it and frees its rasterizer vertices. Decal age is in game ticks at 30 Hz, hence
 * the 1/30 scale. The decal's alpha byte (offset 40) is reset to 0xFF (fully opaque) each frame and
 * only lowered during the fade-out window.
 *
 * DEVIATION: the decompiler renders the fctiwz float→int store as a spurious ">> 24"; the stored byte
 * is simply the truncated 0..255 alpha. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/decal_globals.h"
#include "headers/decal_datum.h"
#include "headers/decal_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int game_time_get(void);
extern void rasterizer_decal_vertices_delete(int cache_index);

void decals_update(void)
{
    if (!global_decal_data->valid)
        return;

    data_iterator iterator;
    data_iterator_new(&iterator, global_decal_data);
    while (data_iterator_next(&iterator))
    {
        int index = iterator.index;
        decal_datum *decal = DATUM_GET(global_decal_data, decal_datum, iterator.index);

        int age_ticks = game_time_get() - decal->creation_time;
        int16_t flags = decal->flags;
        decal->intensity = 0xFF;
        float age_seconds = (float)age_ticks * SECONDS_PER_TICK;

        if ((flags & (1u << _decal_permanent_bit)) != 0)   /* permanent decal — never ages out */
            continue;

        float lifetime = decal->lifetime;
        if (lifetime == 0.0f || age_seconds < lifetime)
        {
            if (lifetime > 0.0f)
            {
                float fade_duration = decal->decay_time;
                if (fade_duration > 0.0f && (lifetime - age_seconds) < fade_duration)
                    decal->intensity = (unsigned char)(int)((lifetime - age_seconds) / fade_duration * 255.0f);
            }
        }
        else   /* expired */
        {
            if ((flags & (1u << _decal_locked_bit)) != 0)   /* locked */
            {
                decal->flags = flags & ~(1u << _decal_locked_bit);
                --decal_globals->locked_count;
            }
            rasterizer_decal_vertices_delete(index);
        }
    }
}
