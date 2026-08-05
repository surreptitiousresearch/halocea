/* unit_drop_current_weapon_to_network @0x836CF8D0 — replicate a unit dropping its current weapon to all
 * loaded client machines: build the message body with the wire-translated unit and dropped-weapon object
 * indices plus the immediate flag, encode it as a stateless "drop current weapon" message-delta, and send
 * it reliably when encoding produced bits. The dropped weapon is the unit's current weapon-slot object
 * (unit.weapon_object_indices[slot]); -1 when the unit holds no weapon. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/field_properties_definition.h"
#include "headers/unit_drop_weapon_message.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_message_type.h"

typedef struct network_game_server network_game_server;

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void unit_drop_current_weapon_to_network(int unit_index, uint8_t immediate)
{
    unit_drop_weapon_message message;
    message.immediate = immediate;
    message.unit_index = field_translated_index_translate_index(&field_properties_object_index_definition, unit_index);
    message.weapon_to_drop = -1;

    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int16_t weapon_slot = unit->unit.current_weapon_index;
    int weapon_object_index = -1;
    if ( weapon_slot != -1 )
        weapon_object_index = unit->unit.weapon_object_indices[weapon_slot];
    message.weapon_to_drop = field_translated_index_translate_index(&field_properties_object_index_definition, weapon_object_index);

    int size_in_bits = message_delta_processor_encode_stateless(
        _message_unit_drop_current_weapon, 0, &message, g_message_encode_buffer, 32760);
    if ( size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        network_game_server_send_message_to_all_loaded_machines(
            server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1, 0, 0, 3);
    }
}
