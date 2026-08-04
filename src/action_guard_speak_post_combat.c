/* action_guard_speak_post_combat @0x83823C78 — trigger the actor's post-combat guard speech line exactly
 * once (the guard_state_data.post_combat_vocalized flag guards re-entry). Maps external_orders.postcombat_type
 * (1-10, matching action_guard_setup_postcombat.c's recognized range) to a speech-line id (48-56); types
 * outside that map do nothing. If the actor has an active guard_look_prop_index, resolves the target unit
 * and a friend/foe tone (2 = non-enemy prop, 3/4 = enemy prop depending on team allegiance) to pass to
 * ai_communication_event. Returns whether a speech line was actually queued.
 *
 * DEVIATION: `_cntlzw(game_team_is_ally(...)) & 0x20) == 0` is the established "value != 0" boolean idiom
 * (cntlzw of 0 is 32=0x20; of any nonzero 32-bit value it's <32) — simplified to a direct `!= 0` comparison. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/ai_information_data.h"
#include "headers/actor_postcombat_type.h"
#include "headers/ai_communication_type.h"
#include "headers/ai_communication_hostility.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_data.h"
extern uint8_t game_team_is_ally(int16_t our_team, int16_t other_team);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);

int action_guard_speak_post_combat(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.action_data.___u0.guard.post_combat_vocalized ) /* post_combat_vocalized */
        return 0;

    __int16 speech_id = -1;
    switch ( actor->external_orders.postcombat_type )
    {
        case actor_postcombat_speak_alone: speech_id = _ai_communication_postcombat_alone; break;
        case actor_postcombat_speak_unscathed: speech_id = _ai_communication_postcombat_unscathed; break;
        case actor_postcombat_speak_wounded: speech_id = _ai_communication_postcombat_wounded; break;
        case actor_postcombat_speak_massacre: speech_id = _ai_communication_postcombat_massacre; break;
        case actor_postcombat_speak_triumph: speech_id = _ai_communication_postcombat_triumph; break;
        case actor_postcombat_check_enemy: speech_id = _ai_communication_postcombat_check_enemy; break;
        case actor_postcombat_check_friend: speech_id = _ai_communication_postcombat_check_friend; break;
        case actor_postcombat_shoot_corpse: speech_id = _ai_communication_postcombat_shoot_corpse; break;
        case actor_postcombat_celebrate: speech_id = _ai_communication_postcombat_celebrate; break;
        default: break;
    }

    int result = 0;
    if ( speech_id != -1 )
    {
        int target_unit_index = -1;
        __int16 tone = -1;

        int guard_look_prop_index = actor->state.action_data.___u0.guard.guard_look_prop_index;
        if ( guard_look_prop_index != -1 )
        {
            prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, guard_look_prop_index);
            target_unit_index = prop->unit_index;
            if ( prop->enemy )
                tone = game_team_is_ally(actor->meta.team_index, prop->team_index) ? _comm_hostility_traitor : _comm_hostility_enemy;
            else
                tone = _comm_hostility_friend;
        }

        ai_communication_event(speech_id, actor->meta.unit_index, target_unit_index, tone, -1, -1, nullptr);
        result = 1;
    }

    actor->state.action_data.___u0.guard.post_combat_vocalized = 1; /* post_combat_vocalized */
    return result;
}
