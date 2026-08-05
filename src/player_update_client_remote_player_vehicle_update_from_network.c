/* player_update_client_remote_player_vehicle_update_from_network @0x8376E870 — client-side decoder for a
 * remote player's *vehicle* update message. Decodes the 64-byte vehicle state record (stateless or
 * incremental against the client baseline), re-orthonormalizes the transmitted forward/up basis, translates
 * the wire vehicle/object index to a local index, and — if the update is in order — either queues it on the
 * remote player's vehicle_update_queue (when it becomes useful within 32 updates) or counts it as outdated.
 * After the second outdated update in a row (threshold is 1 here, versus 2 for positions) the state is
 * applied immediately to the puppet vehicle: position via vehicle_set_position_and_correct_children and the
 * translational/angular velocity + forward/up basis written straight onto the vehicle object.
 *
 * Vehicle sibling of player_update_client_remote_player_position_update_from_network (which has no
 * orientation and only carries a real_point3d).
 *
 * DEVIATIONS (all disasm-resolved, 0x8376E87C-0x8376ECA0):
 *  - The decompiler splits the memcpy'd 64-byte decoded buffer into HIDWORD/LODWORD register puns.
 *    On this big-endian target the HIGH dword of a QWORD is the LOWER address, so the buffer is just the
 *    remote_player_vehicle_update_network_data record laid out normally (vehicle_index@0, position@4,
 *    translational_velocity@0x10, angular_velocity@0x1C, forward@0x28, up@0x34).
 *  - custom_header->update_id / sequence_number are single bytes (lbz), matching the position header.
 *  - The vehicle object's velocity/basis stores land at raw offsets 0x68/0x74/0x80/0x8C, which are exactly
 *    object_datum.object.translational_velocity/forward/up/angular_velocity per the database (an earlier
 *    comment claiming these fields sat at 0x64 was wrong).
 *  - The "***Ignoring [%d (%d)]" trailing varargs are (outdated_in_a_row, 1) pushed on the stack; the
 *    decompiler rendered them as uninitialized locals. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/remote_player_position_update_header.h"
#include "headers/remote_player_vehicle_update_network_data.h"
#include "headers/vehicle_update_entry.h"
#include "headers/vehicle_update_queue.h"
#include "headers/player_datum.h"
#include "headers/object_datum.h"
#include "headers/simple_circular_queue.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/field_properties_definition.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

typedef struct network_game_client network_game_client;


#include "headers/simple_circular_queue.h"
typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t message_delta_processor_decode_incremental(void *decoded_state, void *baseline_state,
        message_delta_processor_header *header, int flags);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t is_remote_player_update_in_order(player_datum *player, const remote_player_position_update_header *player_update_header);
extern uint8_t simple_circular_queue_peek(simple_circular_queue *queue, void **user_data);
extern int simple_circular_queue_get_number_of_queued_entries(const simple_circular_queue *queue);
extern int get_number_of_updates_until_useful(player_datum *player, int candidate_update_id);
extern uint8_t vehicle_update_queue_write(vehicle_update_queue *queue, vehicle_update_entry *entry);
extern void player_update_client_log_specific_remote_player(player_datum *remote_player, int log_level, const char *format, ...);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void vehicle_set_position_and_correct_children(int vehicle_index, real_point3d *new_position);
extern float normalize3d(real_vector3d *v);
extern int game_time_get(void);

/* Re-derive an orthonormal up vector from the transmitted forward/up basis: up' = normalize(forward x up)
 * folded back through forward. Every new component is computed from the original six floats before any
 * store, so the in-place writes are order-independent. Applied identically at 0x8376E918 and 0x8376E9CC. */
static void vehicle_update_reorthonormalize(remote_player_vehicle_update_network_data *decoded)
{
    float fx = decoded->forward.n[0], fy = decoded->forward.n[1], fz = decoded->forward.n[2];
    float ux = decoded->up.n[0], uy = decoded->up.n[1], uz = decoded->up.n[2];

    decoded->up.n[1] = ((((fx * uy) - (ux * fy)) * fx)
                             - (((uz * fy) - (fz * uy)) * fz));
    decoded->up.n[0] = ((((ux * fz) - (uz * fx)) * fz)
                             - (((fx * uy) - (ux * fy)) * fy));
    decoded->up.n[2] = ((((uz * fy) - (fz * uy)) * fy)
                             - (((ux * fz) - (uz * fx)) * fx));

    normalize3d(&decoded->forward);
    normalize3d(&decoded->up);
}

void player_update_client_remote_player_vehicle_update_from_network(message_delta_processor_header *header,
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

    client_remote_player_data *remote = &player->___u26.client_update_data.___u0.remote_player;

    remote_player_vehicle_update_network_data decoded;
    uint8_t decode_ok;
    if ( header->decoding_information->mode )
    {
        /* recovered: *(remote_player_vehicle_update_network_data *)remote->vehicle_update_baseline
         * -> remote->vehicle_update_baseline (member is now the typed struct, not a raw buffer) */
        decoded = remote->vehicle_update_baseline;
        decode_ok = message_delta_processor_decode_incremental(&decoded, &remote->vehicle_update_baseline,
                header, 0);
    }
    else
    {
        decode_ok = message_delta_processor_decode_stateless(&decoded, header);
        if ( decode_ok == 1 )
        {
            vehicle_update_reorthonormalize(&decoded);
            /* recovered: *(remote_player_vehicle_update_network_data *)remote->vehicle_update_baseline = decoded
             * -> remote->vehicle_update_baseline = decoded (typed struct member) */
            remote->vehicle_update_baseline = decoded;
        }
    }

    decoded.vehicle_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
            decoded.vehicle_index);
    vehicle_update_reorthonormalize(&decoded);

    if ( decode_ok != 1 || is_remote_player_update_in_order(player, custom_header) != 1 )
        return;

    if ( remote->last_vehicle_update_id == -1 )
    {
        remote->last_vehicle_update_id = custom_header->update_id;
        return;
    }

    int *peeked_action;
    int pending_action_update_id;
    if ( simple_circular_queue_peek(&remote->action_queue.queue, (void **)&peeked_action) == 1 )
        pending_action_update_id = *peeked_action;
    else
        pending_action_update_id = -1;

    int updates_until_useful = get_number_of_updates_until_useful(player, custom_header->update_id);

    if ( updates_until_useful >= 0 && updates_until_useful < 32 )
    {
        vehicle_update_entry entry;
        entry.client_update_id = custom_header->update_id;
        entry.updates_until_useful = updates_until_useful;
        entry.data = decoded;
        if ( !vehicle_update_queue_write(&remote->vehicle_update_queue, &entry) )
        {
            player_update_client_log_specific_remote_player(player, 1,
                    "[%d]: Remote player vehicle_update_queue overflow.\n", game_time_get());
        }

        int vehicle_entries = simple_circular_queue_get_number_of_queued_entries(
                &remote->vehicle_update_queue.queue);
        int action_entries = simple_circular_queue_get_number_of_queued_entries(&remote->action_queue.queue);
        player_update_client_log_specific_remote_player(player, 1,
                "Received vehicle_update update [%d], on [%d] (%d). [%d] actions, [%d] vehicle updates",
                custom_header->update_id, pending_action_update_id, updates_until_useful,
                action_entries, vehicle_entries);

        remote->number_of_outdated_vehicle_updates_ignored_in_a_row = 0;
        remote->last_position_update_sequence_number = custom_header->sequence_number;
        remote->last_vehicle_update_id = custom_header->update_id;
        return;
    }

    int outdated_in_a_row = remote->number_of_outdated_vehicle_updates_ignored_in_a_row + 1;
    remote->number_of_outdated_vehicle_updates_ignored_in_a_row = outdated_in_a_row;

    if ( outdated_in_a_row <= 1 )
    {
        int position_entries = simple_circular_queue_get_number_of_queued_entries(
                &remote->position_queue.queue);
        int action_entries = simple_circular_queue_get_number_of_queued_entries(&remote->action_queue.queue);
        player_update_client_log_specific_remote_player(player, 1,
                "Received pos update [%d], on [%d] (%d). [%d] actions, [%d] positions ***Ignoring [%d (%d)] ",
                custom_header->update_id, pending_action_update_id, updates_until_useful,
                action_entries, position_entries, outdated_in_a_row, 1);
    }
    else
    {
        int vehicle_entries = simple_circular_queue_get_number_of_queued_entries(
                &remote->vehicle_update_queue.queue);
        int action_entries = simple_circular_queue_get_number_of_queued_entries(&remote->action_queue.queue);
        player_update_client_log_specific_remote_player(player, 1,
                "Received pos update [%d], on [%d] (%d). [%d] actions, [%d] positions ***Applying immediately",
                custom_header->update_id, pending_action_update_id, updates_until_useful,
                action_entries, vehicle_entries);

        int unit_index = player->unit_index;
        remote->number_of_outdated_vehicle_updates_ignored_in_a_row = 0;
        if ( unit_index != -1 )
        {
            /* the puppet unit's parent-vehicle object index, matched against the decoded vehicle before applying */
            object_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
            if ( unit && unit->object.parent_object_index == decoded.vehicle_index )
            {
                object_datum *vehicle = object_try_and_get_and_verify_type(decoded.vehicle_index, object_mask_unit);
                if ( vehicle )
                {
                    vehicle_set_position_and_correct_children(decoded.vehicle_index, &decoded.position);

                    vehicle->object.translational_velocity.n[0] = decoded.translational_velocity.n[0];
                    vehicle->object.translational_velocity.n[1] = decoded.translational_velocity.n[1];
                    vehicle->object.translational_velocity.n[2] = decoded.translational_velocity.n[2];
                    vehicle->object.angular_velocity.n[0] = decoded.angular_velocity.n[0];
                    vehicle->object.angular_velocity.n[1] = decoded.angular_velocity.n[1];
                    vehicle->object.angular_velocity.n[2] = decoded.angular_velocity.n[2];
                    vehicle->object.forward.n[0] = decoded.forward.n[0];
                    vehicle->object.forward.n[1] = decoded.forward.n[1];
                    vehicle->object.forward.n[2] = decoded.forward.n[2];
                    vehicle->object.up.n[0] = decoded.up.n[0];
                    vehicle->object.up.n[1] = decoded.up.n[1];
                    vehicle->object.up.n[2] = decoded.up.n[2];

                    remote->last_position_update_sequence_number = custom_header->sequence_number;
                    remote->last_vehicle_update_id = custom_header->update_id;
                    return;
                }
            }
        }
    }

    remote->last_position_update_sequence_number = custom_header->sequence_number;
    remote->last_vehicle_update_id = custom_header->update_id;
}
