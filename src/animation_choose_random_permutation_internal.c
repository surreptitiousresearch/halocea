/* animation_choose_random_permutation_internal @0x837958C8 — pick a random permutation of an animation by
 * weight. Walks the animation's permutation chain (each entry 180 bytes in the graph's animations block @+0x78;
 * next-permutation index, runtime_normalized_weight) and returns the first whose weight is at or above a
 * random sample. Uses the deterministic (game-state) random stream for game-affecting animations and
 * the local stream otherwise. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"

extern int animation_update_kind_affects_game_state;  /* animation_update_kind enum value */
extern uint32_t *get_global_random_seed_address(void);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state,
                                                     int animation_graph_index, int16_t animation_index)
{
    animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);

    unsigned int *seed = render_or_affects_game_state == animation_update_kind_affects_game_state
                       ? get_global_random_seed_address()
                       : get_global_local_random_seed_address();
    float sample = real_seed_random(seed);

    while ( animation_index != -1 )
    {
        animation *anim = &((animation *)graph->animations.address)[animation_index];
        if ( sample <= anim->runtime_normalized_weight )
            break;
        animation_index = (unsigned __int16)anim->next_animation_index;
    }
    return animation_index;
}
