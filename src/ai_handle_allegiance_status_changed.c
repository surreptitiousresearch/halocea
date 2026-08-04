/* ai_handle_allegiance_status_changed @0x836E8AF8 — when an allegiance between two teams changes state, update
 * every active actor's props that reference the other team. Unless the change is permanent, the prop is marked
 * ally with a pending status change. When the relationship is now friendly (not broken) or the break is
 * permanent, the prop's enemy flag is set accordingly and its unopposable flag and target weight recomputed. */

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

void ai_handle_allegiance_status_changed(int16_t team1_index, int16_t team2_index, uint8_t broken,
                                         uint8_t permanently_broken)
{
    actor_iterator actors;
    actor_iterator_new(&actors, 1u);
    for ( actor_datum *actor = actor_iterator_next(&actors); actor; actor = actor_iterator_next(&actors) )
    {
        __int16 other_team = -1;
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
            if ( prop->team_index != other_team )
                continue;
            if ( !permanently_broken )
            {
                prop->ally = 1;
                prop->ally_status_changed = 1;
            }
            if ( !broken || permanently_broken )
            {
                prop->enemy = broken;
                prop->unopposable_enemy = actor_compute_prop_unopposable(actors.index, props.index);
                prop->target_weight = actor_compute_prop_target_weight(actors.index, props.index);
            }
        }
    }
}
