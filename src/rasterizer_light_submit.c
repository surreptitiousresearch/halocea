/* rasterizer_light_submit @0x837923C0 — append a light to this frame's collected light list (128 max) and
 * forward it to the hardware/D3D-side submit. Returns the assigned slot index, or -1 if full.
 *
 * DEVIATION: the decompiler displays a self-recursive call, but disasm_range(0x837923C0,0x83792428) shows
 * it actually calls a distinct, differently-addressed `_rasterizer_light_submit` (a plain-name-collision
 * artifact, same as multiplayer_sound_from_network.c's `_game_engine_play_multiplayer_sound` case). */

#include <string.h>
#include "headers/rasterizer_lights_t.h"

extern void _rasterizer_light_submit(const rasterizer_light_submit_parameters *parameters);

int rasterizer_light_submit(const rasterizer_light_submit_parameters *parameters)
{
    if ( rasterizer_lights.light_count < 128 )
    {
        int light_count = rasterizer_lights.light_count;

        memcpy(&rasterizer_lights.lights[rasterizer_lights.light_count++], parameters, sizeof(rasterizer_light_submit_parameters));
        _rasterizer_light_submit(parameters);
        return light_count;
    }
    return -1;
}
