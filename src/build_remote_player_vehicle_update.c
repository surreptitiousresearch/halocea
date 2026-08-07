/* build_remote_player_vehicle_update @0x83757374 — on the server, encode an outgoing vehicle-state update
 * for one remote player (the vehicle they currently ride). A full baseline (stateless) update is sent
 * reliably whenever the baseline interval has elapsed (or none was ever sent), stashing the new state as
 * the delta baseline; otherwise, once the regular update interval has elapsed, an incremental
 * (delta-from-baseline) update is sent unreliably. Returns the encoded message size in bits (0 if nothing
 * is sent) and reports reliability via is_reliable_send.
 *
 * The vehicle datum's kinematic fields (translational_velocity, forward, up, angular_velocity) are read
 * as typed object_datum members. The routing header is shared with the position update. */

#include <stdint.h>
#include <string.h>
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/remote_player_position_update_header.h"
#include "headers/remote_player_vehicle_update_network_data.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"


typedef struct _field_properties_definition _field_properties_definition;

extern int game_time_get(void);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

int build_remote_player_vehicle_update(player_datum *source_player, int player_index, uint8_t *is_reliable_send)
{
    *is_reliable_send = 0;

    remote_player_position_update_header header;
    remote_player_vehicle_update_network_data body;
    header.update_id = source_player->___u26.server_update_data.last_completed_update_id;
    header.player_index = field_translated_index_translate_index(&field_properties_player_index_definition, player_index);
    header.sequence_number = source_player->___u26.server_update_data.next_remote_player_update_sequence_number;

    int unit_index = (uint16_t)source_player->unit_index;
    unit_datum *unit_object = (unit_datum *)DATA_ARRAY_ELEMENT(
        object_header_data, object_header_datum, unit_index)->datum;
    int vehicle_index = unit_object->object.parent_object_index;
    object_datum *vehicle = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                               vehicle_index)->datum;

    body.vehicle_index = field_translated_index_translate_index(&field_properties_object_index_definition, vehicle_index);
    body.position = source_player->___u26.server_update_data.position_as_of_last_completed_update;
    body.translational_velocity = vehicle->object.translational_velocity;
    body.angular_velocity = vehicle->object.angular_velocity;
    body.forward = vehicle->object.forward;
    body.up = vehicle->object.up;

    unsigned int baseline_time = source_player->___u26.server_update_data.time_of_last_remote_player_vehicle_baseline_update;
    if (game_time_get() >= baseline_time + remote_player_position_baseline_update_rate || baseline_time == -1)
    {
        int size = message_delta_processor_encode_stateless(_message_remote_player_vehicle_update,
                                                            &header, &body, g_message_encode_buffer, 32760);
        *is_reliable_send = 1;
        /* stash the body as the delta baseline; the decompiler reaches this 0xA0 region via the aliasing
         * client_update_data union member, but on the server it is the vehicle update baseline. */
        /* recovered: baseline passed by value -> &baseline (member is the typed struct, not a buffer) */
        memcpy(&source_player->___u26.server_update_data.remote_player_vehicle_update_baseline, &body, sizeof(body));
        source_player->___u26.server_update_data.time_of_last_remote_player_vehicle_baseline_update = game_time_get();
        source_player->___u26.server_update_data.time_of_last_remote_player_vehicle_update = game_time_get();
        source_player->___u26.server_update_data.update_id_for_vehicle_baseline = header.update_id;
        return size;
    }
    else if (game_time_get() < source_player->___u26.server_update_data.time_of_last_remote_player_vehicle_update
                             + remote_player_vehicle_update_rate)
    {
        return 0;
    }
    else
    {
        int size = message_delta_processor_encode_incremental(_message_remote_player_vehicle_update,
                       &header, &body, &source_player->___u26.server_update_data.remote_player_vehicle_update_baseline,
                       g_message_encode_buffer, 32760, 0);
        *is_reliable_send = 0;
        source_player->___u26.server_update_data.time_of_last_remote_player_vehicle_update = game_time_get();
        return size;
    }
}
