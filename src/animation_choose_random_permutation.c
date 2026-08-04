#include <stdint.h>
#include "headers/animation_update_kind.h"

extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);

int16_t animation_choose_random_permutation(int animation_graph_index, int16_t animation_index)
{
    return animation_choose_random_permutation_internal(
        animation_update_kind_affects_game_state,
        animation_graph_index,
        animation_index);
}
