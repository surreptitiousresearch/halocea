/* build_local_player_vehicle_update @0x83757050 — server-side: build the message-delta body announcing
 * the vehicle state for the vehicle this local player currently rides. Mirrors
 * build_local_player_position_update but carries the ridden vehicle's translated object index and full
 * kinematic state (position, linear/angular velocity, forward/up basis). Throttled by
 * local_player_vehicle_update_rate; returns the encoded size in bits, or 0 when nothing valid to send.
 *
 * The ridden vehicle is found via the player's unit object parent_object_index. The vehicle datum's
 * kinematic fields (translational_velocity, forward, up, angular_velocity) are read as typed object_datum
 * members. The reliable-send flag is always cleared (sent unreliably). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/local_player_vehicle_update_network_data.h"
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t player_update_history_is_valid_id(int update_id);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int game_time_get(void);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern unsigned int GetTickCount(void);
extern void player_update_server_log(const char *format, ...);

int build_local_player_vehicle_update(player_datum *source_player, int player_index,
                                      uint8_t *is_reliable_send)
{
    *is_reliable_send = 0;

    if (player_update_history_is_valid_id(source_player->___u26.server_update_data.last_completed_update_id) != 1)
        return 0;

    int unit_index = source_player->unit_index;
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(
        object_header_data, object_header_datum, unit_index)->datum;
    int vehicle_index = unit_object->object.parent_object_index;
    object_datum *vehicle = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                               vehicle_index)->datum;

    local_player_vehicle_update_network_data body;
    body.sequence_number =
        (uint8_t)source_player->___u26.server_update_data.next_local_player_update_sequence_number;
    body.last_completed_update_id =
        (uint8_t)source_player->___u26.server_update_data.last_completed_update_id;
    body.vehicle_index =
        field_translated_index_translate_index(&field_properties_object_index_definition, vehicle_index);
    body.position = source_player->___u26.server_update_data.position_as_of_last_completed_update;
    body.translational_velocity = vehicle->object.translational_velocity;
    body.angular_velocity = vehicle->object.angular_velocity;
    body.forward = vehicle->object.forward;
    body.up = vehicle->object.up;

    if (source_player->___u26.server_update_data.last_acked_update_id != -1
        && (unsigned int)game_time_get() < source_player->___u26.server_update_data.time_of_last_ack
                                            + local_player_vehicle_update_rate)
    {
        return 0;
    }

    int size_in_bits = message_delta_processor_encode_stateless(
        _message_local_player_vehicle_update, nullptr, &body, g_message_encode_buffer, 32760);
    *is_reliable_send = 0;

    int last_completed = source_player->___u26.server_update_data.last_completed_update_id;
    source_player->___u26.server_update_data.next_local_player_update_sequence_number =
        (source_player->___u26.server_update_data.next_local_player_update_sequence_number + 1) % 32;

    int now = game_time_get();
    unsigned int tick = GetTickCount();
    player_update_server_log("[%d]: [%d]:\t Acked vehicle [%d]\n", tick, now, last_completed);

    source_player->___u26.server_update_data.last_acked_update_id =
        source_player->___u26.server_update_data.last_completed_update_id;
    source_player->___u26.server_update_data.time_of_last_ack = game_time_get();
    return size_in_bits;
}
