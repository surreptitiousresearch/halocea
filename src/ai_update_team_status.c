/* ai_update_team_status @0x836E8D28 — recompute every active actor's perception of every prop it holds: refresh
 * each prop's cached team index from the unit, then its enemy/ally/unopposable-enemy flags and target weight
 * relative to the owning actor's team. Iterates all active actors and their props. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/actor_iterator.h"
#include "headers/prop_iterator.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/blam_data_globals.h"


extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern uint8_t game_team_is_ally(int16_t our_team, int16_t other_team);
extern uint8_t actor_compute_prop_unopposable(int actor_index, int prop_index);
extern float actor_compute_prop_target_weight(int actor_index, int prop_index);

void ai_update_team_status(void)
{
    actor_iterator actors;
    actor_iterator_new(&actors, 1u);
    actor_datum *actor = actor_iterator_next(&actors);
    for ( ; actor; actor = actor_iterator_next(&actors) )
    {
        prop_iterator props;
        prop_iterator_new(&props, actors.index);
        for ( prop_datum *prop = prop_iterator_next(&props); prop; prop = prop_iterator_next(&props) )
        {
            int16_t team = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop->unit_index)->datum)->object.owner_team_index;
            prop->team_index = team;
            prop->enemy = game_team_is_enemy(actor->meta.team_index, team);
            prop->ally = game_team_is_ally(actor->meta.team_index, prop->team_index);
            prop->unopposable_enemy = actor_compute_prop_unopposable(actors.index, props.index);
            prop->target_weight = actor_compute_prop_target_weight(actors.index, props.index);
        }
    }
}
