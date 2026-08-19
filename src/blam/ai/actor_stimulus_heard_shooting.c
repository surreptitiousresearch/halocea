/* actor_stimulus_heard_shooting @0x837D4C38 — react to hearing weapon fire reported by a perception prop.
 * If the prop is an enemy the actor enters combat against it directly; otherwise the firing is from a
 * friend, so the actor enters "friend in combat" and tries to inherit the friend's target: either the
 * friend's combat target (when sufficiently engaged) or, if a player recently fired at an enemy, an
 * orphan target derived from that player. In all cases the actor glances toward the prop. */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/prop_datum.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/actor_combat_status.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/direction_specification.h"
#include "headers/direction_specification_type.h"
#include "headers/secondary_look_type.h"
#include "headers/secondary_look_priority.h"
#include "headers/blam_data_globals.h"


extern void actor_stimulus_enter_combat_perceived_enemy(int actor_index, int prop_index);
extern void actor_stimulus_enter_combat_friend_in_combat(int actor_index, int prop_index);
extern void actor_derive_target_information(int actor_index, int friend_actor_index);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern int actor_perception_create_orphan_from_friend(int actor_index, int unit_index, int friend_actor_index, int friend_prop_index);
extern int game_time_get(void);
extern direction_specification *direction_get_empty(direction_specification *result);
extern uint8_t actor_look_secondary(int actor_index, int16_t type, int16_t priority, direction_specification *direction);

void actor_stimulus_heard_shooting(int actor_index, int prop_index)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);
    if (prop->enemy)
    {
        actor_stimulus_enter_combat_perceived_enemy(actor_index, prop_index);
    }
    else
    {
        char *object = ((char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, prop->unit_index)->datum);
        actor_stimulus_enter_combat_friend_in_combat(actor_index, prop_index);

        int controlling_player_index = ((unit_datum *)object)->unit.player_index;
        if (controlling_player_index == -1)
        {
            int friend_actor_index = ((unit_datum *)object)->unit.actor_index;
            if (friend_actor_index != -1
                && DATA_ARRAY_ELEMENT(actor_data, actor_datum, friend_actor_index)->state.combat_status >= _actor_combat_status_certain)
                actor_derive_target_information(actor_index, friend_actor_index);
        }
        else
        {
            player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, controlling_player_index);
            if (player->aim_assist_unit_index != -1
                && player->aim_assist_timestamp + 90 >= game_time_get()
                && game_team_is_enemy(
                       DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.team_index,
                       ((object_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->aim_assist_unit_index)->datum)->object.owner_team_index))
                actor_perception_create_orphan_from_friend(actor_index, player->aim_assist_unit_index, -1, -1);
        }
    }

    direction_specification empty;
    direction_specification look_direction = *direction_get_empty(&empty);
    look_direction.type = _direction_specification_prop;
    look_direction.___u1.prop_index = prop_index;
    actor_look_secondary(actor_index, _secondary_look_shooting_prop, _secondary_look_priority_default, &look_direction);
}
