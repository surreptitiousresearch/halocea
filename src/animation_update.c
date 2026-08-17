/* animation_update @0x836C6B18 */
#include <stdint.h>
#include "headers/animation_update_kind.h"
#include "headers/animation_state.h"

extern int16_t animation_update_internal(animation_update_kind render_or_affects_game_state, int animation_graph_index, animation_state *state, int *triggered_sound_index);

int16_t animation_update(int animation_graph_index, animation_state *state, int *triggered_sound_index)
{
    return animation_update_internal(animation_update_kind_affects_game_state,
                                     animation_graph_index,
                                     state,
                                     triggered_sound_index);
}
