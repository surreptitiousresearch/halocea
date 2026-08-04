/* find_next_target @0x838151B8 — pick a new random "special target" (infection/juggernaut-style hunted
 * player) for `my_index`, distinct from its current target, on an opposing team, with an active unit.
 * Counts eligible players, picks one uniformly at random via seed_random_range, stores the chosen player
 * index into my_index's player_datum.multiplayer.special, and (if one was found) announces it via
 * game_show_score_extended (slayer_message_new_target). Leaves the target as -1 if no eligible player exists. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/slayer_message.h"
#include "headers/blam_data_globals.h"


extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void game_show_score_extended(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

static unsigned __int8 target_is_eligible(player_datum *me, int current_target, int my_index, player_datum *candidate,
        int candidate_index)
{
    return candidate_index != my_index && candidate_index != current_target
        && candidate->team_index != me->team_index && candidate->unit_index != -1;
}

void find_next_target(int my_index)
{
    player_datum *me = DATA_ARRAY_ELEMENT(player_data, player_datum, my_index);
    int current_target = me->multiplayer.special;
    int eligible_count = 0;
    int index = -1;

    data_iterator player_iterator;
    data_iterator_new(&player_iterator, player_data);

    for ( player_datum *candidate = data_iterator_next(&player_iterator); candidate;
            candidate = data_iterator_next(&player_iterator) )
    {
        player_datum *candidate_ptr = DATA_ARRAY_ELEMENT(player_data, player_datum, player_iterator.index);
        if ( target_is_eligible(me, current_target, my_index, candidate_ptr, player_iterator.index) )
            ++eligible_count;
    }

    if ( eligible_count > 0 )
    {
        __int16 chosen = seed_random_range(get_global_random_seed_address(), 0, eligible_count);

        data_iterator_new(&player_iterator, player_data);
        while ( data_iterator_next(&player_iterator) )
        {
            player_datum *candidate_ptr = DATA_ARRAY_ELEMENT(player_data, player_datum, player_iterator.index);
            if ( target_is_eligible(me, current_target, my_index, candidate_ptr, player_iterator.index) )
            {
                if ( !chosen )
                {
                    index = player_iterator.index;
                    break;
                }
                --chosen;
            }
        }
    }

    me->multiplayer.special = index;
    if ( index != -1 )
        game_show_score_extended(my_index, my_index, slayer_message_new_target, index, 1u);
}
