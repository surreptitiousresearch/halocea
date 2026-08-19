/* encounter_post_combat_select_random_behavior @0x8370B208 — pick one of an encounter's up-to-4
 * weighted post-combat behavior possibilities (only every other row of the 4x2 possibilities grid is
 * considered — column 0 of each of the 4 rows). If more than one candidate has positive weight and a
 * valid actor, weighted-randomly select among them; otherwise fall back to the single (or last-seen)
 * valid candidate. Copies the chosen possibility into *selected_possibility and returns its index
 * (-1 if none was valid). */

#include <stdint.h>
#include "headers/post_combat_possibility.h"

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);

int16_t encounter_post_combat_select_random_behavior(const post_combat_possibility (*possibilities)[2],
    post_combat_possibility *selected_possibility)
{
    int chosen_index = -1;
    int valid_count = 0;
    float total_weight = 0.0f;

    for ( int row = 0; row < 4; ++row )
    {
        const post_combat_possibility *candidate = &(*possibilities)[2 * row];
        if ( candidate->weight > 0.0f && candidate->actor_index != -1 )
        {
            total_weight += candidate->weight;
            chosen_index = row;
            ++valid_count;
        }
    }

    if ( valid_count > 1 )
    {
        unsigned int *random_seed = get_global_random_seed_address();
        float roll = real_seed_random(random_seed) * total_weight;

        /* chosen_index is deliberately NOT reset here: if the weighted roll never triggers (a floating-point
         * edge case near total_weight), the shipped code falls back to the last valid row found above. */
        float running_weight = 0.0f;

        for ( int row = 0; row < 4; ++row )
        {
            const post_combat_possibility *candidate = &(*possibilities)[2 * row];
            if ( candidate->weight > 0.0f && candidate->actor_index != -1 )
            {
                running_weight += candidate->weight;
                if ( running_weight > roll )
                {
                    chosen_index = row;
                    break;
                }
            }
        }
    }

    if ( chosen_index != -1 )
    {
        const post_combat_possibility *chosen = &(*possibilities)[2 * chosen_index];
        selected_possibility->actor_index = chosen->actor_index;
        selected_possibility->weight = chosen->weight;
        selected_possibility->prop_index = chosen->prop_index;
        selected_possibility->unit_index = chosen->unit_index;
    }

    return chosen_index;
}
