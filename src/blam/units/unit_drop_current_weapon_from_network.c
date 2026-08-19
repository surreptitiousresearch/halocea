/* unit_drop_current_weapon_from_network @0x836D25F8 — apply a stateless "unit drop current weapon"
 * message from the network: decode a unit_drop_weapon_message payload, resolve the translated unit index
 * back to a local one, and verify it as a unit. If the sender's dropped-weapon index doesn't match the
 * unit's current weapon slot, search the unit's 4-slot weapon inventory (offset +760, same raw-offset
 * convention established in unit_drop_current_weapon_to_network.c) for that weapon and, if found, set it
 * as the desired weapon slot (offset +756) and ask the unit to ready it. Finally, if the unit's current
 * weapon slot (offset +754, re-read after the ready-desired-weapon call) now resolves to the sender's
 * weapon, drop it.
 *
 */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/field_properties_definition.h"
#include "headers/unit_drop_weapon_message.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_ready_desired_weapon(int unit_index, uint8_t immediate);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);

void unit_drop_current_weapon_from_network(message_delta_processor_header *header)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    unit_drop_weapon_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    int unit_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
        message.unit_index); /* DB member name */
    if ( unit_index == -1 )
        return;

    unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( !unit )
        return;

    int weapon_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
        message.weapon_to_drop); /* DB member name */

    int current_weapon_slot_index = unit->unit.current_weapon_index;
    int current_weapon_object_index = -1;
    if ( current_weapon_slot_index != -1 )
        current_weapon_object_index = unit->unit.weapon_object_indices[current_weapon_slot_index];

    if ( current_weapon_object_index != weapon_index )
    {
        uint8_t found = 0;
        int slot;

        for ( slot = 0; unit->unit.weapon_object_indices[slot] != weapon_index; ++slot )
        {
            if ( slot >= 4 )
                goto weapon_not_found;
        }

        found = 1;
        unit->unit.desired_weapon_index = slot;

weapon_not_found:
        if ( found )
            unit_ready_desired_weapon(unit_index, 1u);
    }

    current_weapon_slot_index = unit->unit.current_weapon_index;
    current_weapon_object_index = -1;
    if ( current_weapon_slot_index != -1 )
        current_weapon_object_index = unit->unit.weapon_object_indices[current_weapon_slot_index];

    if ( current_weapon_object_index == weapon_index )
        unit_drop_current_weapon(unit_index, message.immediate);
}
