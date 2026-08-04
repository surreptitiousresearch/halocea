/* unit_animation_update @0x836CC118 — advance one of a unit's playing animation_states (base, overlay,
 * soft-ping, etc.) by one tick via animation_update_internal, then, if that animation triggered a sound this
 * tick (triggered_sound_index != -1), plays it as a 3D impulse sound at the world origin.
 *
 * DEVIATION: the decompiler's call to object_impulse_sound_new shows a bogus final `0x82000000` literal
 * argument — disasm (0x836CC118-0x836CC18C) has no such instruction; that value is the raw address of the
 * `global_origin3d`/`global_forward3d` symbols leaking into the decompiler's rendering, not a real argument.
 * The real call is the DB's 7-arg prototype: position/forward are the global-origin/forward constants (this
 * sound has no real per-unit position — it's not attached to a specific world location), scale is 1.0,
 * is_local_player is 0. */

#include <stdint.h>
#include "headers/animation_state.h"
#include "headers/animation_update_kind.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern int16_t animation_update_internal(animation_update_kind render_or_affects_game_state, int animation_graph_index, animation_state *state, int *triggered_sound_index);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);

int16_t unit_animation_update(int unit_index, int animation_graph_index, animation_state *state)
{
    int triggered_sound_index;
    int updated = animation_update_internal(animation_update_kind_affects_game_state, animation_graph_index, state,
            &triggered_sound_index);

    if (triggered_sound_index != -1)
        object_impulse_sound_new(unit_index, triggered_sound_index, 0, global_origin3d, global_forward3d, 1.0f, 0);

    return (int16_t)updated;
}
