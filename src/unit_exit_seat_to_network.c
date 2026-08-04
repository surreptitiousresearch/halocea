/* unit_exit_seat_to_network @0x836CA9B8 — replicate a "player exits vehicle" event. Translates the unit's
 * object index into a network index, encodes a stateless _message_player_exit_vehicle delta carrying that
 * index (and the force-exit flag), and, on a server, sends it to all loaded machines.
 *
 * The network message subsystem is an extern boundary (see unit_kill_to_network). */

#include <stdint.h>
#include "headers/iterated_message.h"   /* message_delta_processor_message_definition_type */
#include "headers/field_properties_definition.h"
#include "headers/network_game_server.h"
typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

#define _message_player_exit_vehicle       9    /* DB enum: message-delta definition type */
#define network_message_type_message_delta 1

#include "headers/player_exit_vehicle_message.h"
#include "headers/blam_data_globals.h"

void unit_exit_seat_to_network(int object_index, uint8_t force_exit)
{
    player_exit_vehicle_message message; /* typedef, not a struct tag */
    message.object_index =
        field_translated_index_translate_index(&field_properties_object_index_definition, object_index);
    message.forced_exit = force_exit;

    int size_in_bits = message_delta_processor_encode_stateless(_message_player_exit_vehicle, 0,
                           &message.object_index, g_message_encode_buffer, 0x7FF8);
    if ( size_in_bits > 0 )
    {
        network_game_server_send_message_to_all_loaded_machines(global_network_game_server_get(),
            network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1, 0, 0, 3);
    }
}
