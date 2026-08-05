/* ai_handle_allegiance_broken_notification @0x836E8C18 — after an allegiance between two teams breaks, update
 * every active actor on either team: for each prop that references the now-hostile other team, mark it
 * enemy/ally and recompute its unopposable flag and target weight. Finally notify the allegiance-change system.
 */

#include <stdint.h>
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"
#include "headers/prop_iterator.h"
#include "headers/prop_datum.h"

extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t actor_compute_prop_unopposable(uint16_t actor_index, int prop_index);
extern float actor_compute_prop_target_weight(int actor_index, int prop_index);
extern void game_allegiance_notify_change(int16_t aggressor_team_index, int16_t victim_team_index);

void ai_handle_allegiance_broken_notification(int16_t team1_index, int16_t team2_index, uint8_t broken)
{
    actor_iterator actors;
    actor_iterator_new(&actors, 1u);
    for ( actor_datum *actor = actor_iterator_next(&actors); actor; actor = actor_iterator_next(&actors) )
    {
        int16_t other_team = -1;
        int team_index = actor->meta.team_index;
        if ( team_index == team1_index )
            other_team = team2_index;
        else if ( team_index == team2_index )
            other_team = team1_index;
        if ( other_team == -1 )
            continue;

        prop_iterator props;
        prop_iterator_new(&props, actors.index);
        for ( prop_datum *prop = prop_iterator_next(&props); prop; prop = prop_iterator_next(&props) )
        {
            if ( prop->team_index == other_team )
            {
                prop->enemy = broken;
                prop->ally = 1;
                prop->ally_status_changed = 0;
                prop->unopposable_enemy = actor_compute_prop_unopposable(actors.index, props.index);
                prop->target_weight = actor_compute_prop_target_weight(actors.index, props.index);
            }
        }
    }
    game_allegiance_notify_change(team1_index, team2_index);
}
