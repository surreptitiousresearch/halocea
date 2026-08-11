/* actor_action_deny_transition @0x837F22A8 — decide whether an actor's current action forbids a state
 * transition this frame. An active timed action (word @ actor +144 set, remaining @ +146 > 0) denies;
 * an active encounter squad delay timer denies while the actor's action stage (word @ actor +110) is
 * still early (< 5), otherwise it expires the squad timer. A guard/charge action (word @ +108 == 11)
 * denies based on its sub-flags (bytes @ +158 / +161). Actor records are stride 1828. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/encounter_datum.h"
#include "headers/squad_datum.h"
#include "headers/actor_action.h"
#include "headers/actor_combat_status.h"
#include "headers/blam_data_globals.h"

extern void encounter_squad_timer_expire(int encounter_index, int16_t squad_index);

uint8_t actor_action_deny_transition(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int deny = 0;
    if ( (uint16_t)actor->state.command_list_index != 0xFFFF )   /* DEVIATION: int16_t field — uncast, the promoted -1 made `!= 0xFFFF` always TRUE and the delay timer was read with no command list; binary zero-extends, lhz r8,0x90(r31) @0x837F22D8 + cmplwi cr6,r8,0xFFFF @0x837F22DC */
        deny = actor->state.command_list_delay_timer > 0;

    int encounter_index = actor->meta.encounter_index;
    if ( encounter_index != -1 )
    {
        short squad_member_index = actor->meta.squad_index;
        short first_squad_index = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->squad_base;
        if ( squad_array[(short)(first_squad_index + squad_member_index)].delay_timer > 0 )
        {
            if ( actor->state.combat_status < _actor_combat_status_clear_los )
                deny = 1;
            else
                encounter_squad_timer_expire(encounter_index, squad_member_index);
        }
    }

    if ( actor->state.action != actor_action_obey )
        return deny;
    if ( actor->state.action_data.___u0.obey.initiative )
        return deny;
    if ( actor->state.action_data.___u0.obey.finished )
        return deny;
    return 1;
}
