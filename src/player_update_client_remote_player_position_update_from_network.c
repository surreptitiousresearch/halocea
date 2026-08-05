/* player_update_client_remote_player_position_update_from_network @0x8376E4C8 — client-side decoder for a
 * remote player's position update message. Decodes the 12-byte position record (stateless or incremental
 * against the client baseline), and if the update is in order either queues it on the remote player's
 * position_queue (when it will become useful within 32 updates) or counts it as outdated — after three
 * outdated updates in a row the position is applied to the puppet unit immediately (unless it is within
 * 1 world unit of the current position, "saved by tolerance"). Sibling of
 * apply_remote_player_position_update / player_update_server_remote_player_action_update_to_network.
 *
 * DEVIATIONS (all disasm-resolved): the decompiler picked the server arm of the player update-data union
 * for several client fields — +0x174 is client position_update_baseline (not
 * time_of_last_remote_player_position_update; the record has no timestamp, it is just a real_point3d),
 * +0x170 is client last_position_update_id (not next_remote_player_update_sequence_number), +0x198 is
 * client number_of_outdated_position_updates_ignored_in_a_row (not remote_player_vehicle_update_baseline).
 * position_queue_write's by-value real_point3d was rendered as a LODWORD/__ROL4__ pun. The "Ignoring" log's
 * v28/v29 are the stack varargs (the outdated counter and the constant 2), not uninitialized locals. The
 * dist<=1 branch logs a different string ("saved by tolerance") that the decompiler rendered as
 * HIDWORD garbage. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/remote_player_position_update_header.h"
#include "headers/remote_player_position_update_network_data.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/position_queue.h"
#include "headers/simple_circular_queue.h"
#include "headers/networked_datum_role.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/field_properties_definition.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

typedef struct network_game_client network_game_client;


#include "headers/simple_circular_queue.h"
typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t message_delta_processor_decode_incremental(real_point3d *decoded_state,
        void *baseline_state, message_delta_processor_header *header, int flags);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t is_remote_player_update_in_order(player_datum *player, const remote_player_position_update_header *player_update_header);
extern uint8_t simple_circular_queue_peek(simple_circular_queue *queue, void **user_data);
extern int simple_circular_queue_get_number_of_queued_entries(const simple_circular_queue *queue);
extern int get_number_of_updates_until_useful(player_datum *player, int candidate_update_id);
extern uint8_t position_queue_write(position_queue *queue, real_point3d server_position_as_of_this_update, int client_update_id, int updates_until_useful);
extern void player_update_client_log_specific_remote_player(player_datum *remote_player, int log_level, const char *format, ...);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void update_remote_player_with_server_position(int unit_index, unit_datum *unit, real_point3d *position);
extern int game_time_get(void);
extern float __fsqrts(float x);

void player_update_client_remote_player_position_update_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    const remote_player_position_update_header *custom_header =
            (const remote_player_position_update_header *)header->custom_header;

    player_datum *player = datum_try_and_get(player_data,
            field_translated_index_get_local_index(&field_properties_player_index_definition,
                    custom_header->player_index));
    if ( !player )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    remote_player_position_update_network_data *baseline = &player->___u26.client_update_data.___u0.remote_player.position_update_baseline;

    real_point3d decoded_position;
    uint8_t decoded;
    if ( header->decoding_information->mode )
    {
        decoded_position = baseline->position;
        decoded = message_delta_processor_decode_incremental(&decoded_position, baseline, header, 0);
    }
    else
    {
        decoded = message_delta_processor_decode_stateless(&decoded_position, header);
        if ( decoded == 1 )
            baseline->position = decoded_position;
    }

    if ( decoded != 1 || is_remote_player_update_in_order(player, custom_header) != 1 )
        return;

    if ( player->___u26.client_update_data.___u0.remote_player.last_position_update_id != -1 )
    {
        int *peeked_action;
        int pending_action_update_id;
        if ( simple_circular_queue_peek(&player->___u26.client_update_data.___u0.remote_player.action_queue.queue,
                (void **)&peeked_action) == 1 )
            pending_action_update_id = *peeked_action;
        else
            pending_action_update_id = -1;

        int updates_until_useful = get_number_of_updates_until_useful(player, custom_header->update_id);

        if ( updates_until_useful >= 0 && updates_until_useful < 32 )
        {
            if ( !position_queue_write(&player->___u26.client_update_data.___u0.remote_player.position_queue,
                    decoded_position, custom_header->update_id, updates_until_useful) )
            {
                player_update_client_log_specific_remote_player(player, 1,
                        "[%d]: Remote player position_queue overflow.\n", game_time_get());
            }

            int position_entries = simple_circular_queue_get_number_of_queued_entries(
                    &player->___u26.client_update_data.___u0.remote_player.position_queue.queue);
            int action_entries = simple_circular_queue_get_number_of_queued_entries(
                    &player->___u26.client_update_data.___u0.remote_player.action_queue.queue);
            player_update_client_log_specific_remote_player(player, 1,
                    "Received pos update [%d], on [%d] (%d). [%d] actions, [%d] positions",
                    custom_header->update_id, pending_action_update_id, updates_until_useful,
                    action_entries, position_entries);

            player->___u26.client_update_data.___u0.remote_player.number_of_outdated_position_updates_ignored_in_a_row = 0;
            player->___u26.client_update_data.___u0.remote_player.last_position_update_sequence_number =
                    custom_header->sequence_number;
            player->___u26.client_update_data.___u0.remote_player.last_position_update_id = custom_header->update_id;
            return;
        }

        int outdated_in_a_row =
                player->___u26.client_update_data.___u0.remote_player.number_of_outdated_position_updates_ignored_in_a_row + 1;
        player->___u26.client_update_data.___u0.remote_player.number_of_outdated_position_updates_ignored_in_a_row =
                outdated_in_a_row;

        if ( outdated_in_a_row <= 2 )
        {
            int position_entries = simple_circular_queue_get_number_of_queued_entries(
                    &player->___u26.client_update_data.___u0.remote_player.position_queue.queue);
            int action_entries = simple_circular_queue_get_number_of_queued_entries(
                    &player->___u26.client_update_data.___u0.remote_player.action_queue.queue);
            player_update_client_log_specific_remote_player(player, 1,
                    "Received pos update [%d], on [%d] (%d). [%d] actions, [%d] positions ***Ignoring [%d (%d)] ",
                    custom_header->update_id, pending_action_update_id, updates_until_useful,
                    action_entries, position_entries, outdated_in_a_row, 2);
        }
        else
        {
            int position_entries = simple_circular_queue_get_number_of_queued_entries(
                    &player->___u26.client_update_data.___u0.remote_player.position_queue.queue);
            int action_entries = simple_circular_queue_get_number_of_queued_entries(
                    &player->___u26.client_update_data.___u0.remote_player.action_queue.queue);
            player_update_client_log_specific_remote_player(player, 1,
                    "Received pos update [%d], on [%d] (%d). [%d] actions, [%d] positions ***Applying immediately",
                    custom_header->update_id, pending_action_update_id, updates_until_useful,
                    action_entries, position_entries);

            int unit_index = player->unit_index;
            player->___u26.client_update_data.___u0.remote_player.number_of_outdated_position_updates_ignored_in_a_row = 0;
            if ( unit_index != -1 )
            {
                unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
                if ( unit
                  && unit->object.parent_object_index == -1
                  && unit->object.datum_role == _networked_datum_puppet )
                {
                    float delta_z = decoded_position.n[2] - unit->object.position.n[2];
                    float delta_x = decoded_position.n[0] - unit->object.position.n[0];
                    float delta_y = decoded_position.n[1] - unit->object.position.n[1];
                    float distance = __fsqrts((delta_y * delta_y)
                            + ((delta_x * delta_x) + (delta_z * delta_z)));

                    if ( distance > 1.0 )
                    {
                        player_update_client_log_specific_remote_player(player, 1,
                                "Apply immediately dist: [%f] (%f)", (double)distance, 1.0);
                        update_remote_player_with_server_position(player->unit_index, unit, &decoded_position);
                    }
                    else
                    {
                        /* different string than the far branch — decompiler rendered it as HIDWORD garbage */
                        player_update_client_log_specific_remote_player(player, 1,
                                "Apply immediately saved by tolerance [%f] (%f)", (double)distance, 1.0);
                    }

                    player->___u26.client_update_data.___u0.remote_player.last_position_update_sequence_number =
                            custom_header->sequence_number;
                    player->___u26.client_update_data.___u0.remote_player.last_position_update_id = custom_header->update_id;
                    return;
                }
            }
        }

        player->___u26.client_update_data.___u0.remote_player.last_position_update_sequence_number =
                custom_header->sequence_number;
    }

    player->___u26.client_update_data.___u0.remote_player.last_position_update_id = custom_header->update_id;
}
