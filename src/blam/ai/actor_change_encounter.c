/* actor_change_encounter @0x8371EA90 — move an actor to a new encounter+squad. Cancels any active
 * pending firing-position/move-position destination (destination_type 3/4), flushes its position indices,
 * detaches it from its current encounter
 * (or the encounterless pool), then attaches it to the target encounter+squad (or the encounterless pool if
 * encounter_index is -1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/destination_type.h"
#include "headers/blam_data_globals.h"

extern void actor_action_flush_position_indices(int actor_index);
extern void encounterless_detach_actor(int actor_index);
extern void encounter_detach_actor(int actor_index, uint8_t died);
extern void encounterless_attach_actor(int actor_index);
extern void encounter_attach_actor(int actor_index, int encounter_index, int16_t squad_index, uint8_t has_previous_team);

void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t action_mode = actor->control.path.destination_orders.destination_type;   /* actor +0x46C */
    actor->firing_positions.current_position_index = -1;                             /* actor +0x3B8 */
    if ( action_mode == _destination_firing_position || action_mode == _destination_move_position )
    {
        actor->control.path.destination_orders.ignore_target_object_index = -1;     /* actor +0x480 */
        actor->control.path.destination_orders.destination_type = _destination_none;
    }
    actor_action_flush_position_indices(actor_index);

    if ( actor->meta.encounterless )                                                 /* meta.encounterless */
        encounterless_detach_actor(actor_index);
    else if ( actor->meta.encounter_index != -1 )                                   /* meta.encounter_index */
        encounter_detach_actor(actor_index, 0);

    if ( encounter_index == -1 )
        encounterless_attach_actor(actor_index);
    else
        encounter_attach_actor(actor_index, encounter_index, squad_index, 1u);
}
