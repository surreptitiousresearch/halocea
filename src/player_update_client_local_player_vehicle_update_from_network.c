/* player_update_client_local_player_vehicle_update_from_network @0x8376DE80 — vehicle-update counterpart of
 * player_update_client_local_player_update_from_network.c: decode a stateless "local player vehicle update
 * ack" from the server and apply it to this client's local player slot. The decoded body carries the ridden
 * vehicle's kinematic state; before use, its up vector is re-orthogonalized against forward
 * ((forward x up) x forward) and both are normalized. The local player's own player_datum is fetched, the ack
 * is dropped if stale (is_local_player_update_in_order), and on acceptance the client's reception state
 * (last_action_update_id / action_baseline_id / action_update_baseline = the decoded position) is refreshed
 * and the update history is replayed with the decoded vehicle body as the starting vehicle info.
 *
 * DEVIATION (all confirmed via disasm 0x8376DEB0-0x8376DFE0):
 *  - The DB/decompiler surfaced a phantom float arg to players_local_player_get (it is arg-less; the value
 *    was leftover from the preceding normalize3d — no register is set up for it).
 *  - The 12-byte real_point3d starting_position passed by value to player_update_history_play spans two GPRs
 *    (r6=x,y / r7=z); the decompiler mis-rendered it as a `+65` pointer deref plus a ROL-wrapped 5th arg.
 *    Resolved: history=NULL, unit_index=player->unit_index, last_completed_update_id=action_baseline_id,
 *    starting_position=the decoded position, vehicle_starting_info=&the decoded body, destructive=1,
 *    show_results=0.
 *  - The translated object index is computed but not consulted afterward (the local player is found via
 *    players_local_player_get, not by object index); reproduced faithfully.
 *  - Player reception-state offsets (0xF8/0xFC/0x100) reuse the client_update_data.remote_player field names
 *    established in the action sibling. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/player_update_history.h"
#include "headers/local_player_vehicle_update_network_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/field_properties_definition.h"
#include "headers/client_log_subject.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
#include "headers/player_update_history.h"
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern float normalize3d(real_vector3d *v);
extern int players_local_player_get(void);
extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t is_local_player_update_in_order(int sequence_number, int last_update_sequence_number);
extern int game_time_get(void);
extern void player_update_client_log(client_log_subject subject, int log_level, const char *format, ...);
extern network_game_client *global_network_game_client_get(void);
extern void player_update_history_play(player_update_history *history, int unit_index, int last_completed_update_id, real_point3d starting_position, local_player_vehicle_update_network_data *vehicle_starting_info, uint8_t destructive, uint8_t show_results);

void player_update_client_local_player_vehicle_update_from_network(message_delta_processor_header *header,
    network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    local_player_vehicle_update_network_data vehicle_update;
    if ( message_delta_processor_decode_stateless(&vehicle_update, header) != 1 )
        return;

    vehicle_update.vehicle_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
        vehicle_update.vehicle_index);

    /* re-orthogonalize up as (forward x up) x forward, then normalize both basis vectors. */
    real_vector3d forward_cross_up;
    forward_cross_up.n[0] = vehicle_update.forward.n[1] * vehicle_update.up.n[2]
                          - vehicle_update.forward.n[2] * vehicle_update.up.n[1];
    forward_cross_up.n[1] = vehicle_update.forward.n[2] * vehicle_update.up.n[0]
                          - vehicle_update.forward.n[0] * vehicle_update.up.n[2];
    forward_cross_up.n[2] = vehicle_update.forward.n[0] * vehicle_update.up.n[1]
                          - vehicle_update.forward.n[1] * vehicle_update.up.n[0];

    vehicle_update.up.n[0] = forward_cross_up.n[1] * vehicle_update.forward.n[2]
                           - forward_cross_up.n[2] * vehicle_update.forward.n[1];
    vehicle_update.up.n[1] = forward_cross_up.n[2] * vehicle_update.forward.n[0]
                           - forward_cross_up.n[0] * vehicle_update.forward.n[2];
    vehicle_update.up.n[2] = forward_cross_up.n[0] * vehicle_update.forward.n[1]
                           - forward_cross_up.n[1] * vehicle_update.forward.n[0];

    normalize3d(&vehicle_update.forward);
    normalize3d(&vehicle_update.up);

    player_datum *player = datum_try_and_get(player_data, players_local_player_get());
    if ( !player )
        return;

    if ( is_local_player_update_in_order(vehicle_update.sequence_number,
             player->___u26.client_update_data.___u0.remote_player.last_action_update_id) == 1 )
    {
        player_update_client_log(_client_log_local_player, 1, "[%d]: Received vehicle ack for update [%d].\n",
            game_time_get(), vehicle_update.last_completed_update_id);

        player->___u26.client_update_data.___u0.remote_player.last_action_update_id = vehicle_update.sequence_number;
        player->___u26.client_update_data.___u0.remote_player.action_baseline_id = vehicle_update.last_completed_update_id;
        *(real_point3d *)&player->___u26.client_update_data.___u0.remote_player.action_update_baseline = vehicle_update.position;

        global_network_game_client_get();

        player_update_history_play(nullptr, player->unit_index,
            player->___u26.client_update_data.___u0.remote_player.action_baseline_id, vehicle_update.position, &vehicle_update,
            1u, 0);
    }
    else
    {
        player_update_client_log(_client_log_local_player, 1,
            "[%d]: Threw away local player vehicle ack [%d] (%d), previous ack [%d] (%d).\n",
            game_time_get(), vehicle_update.last_completed_update_id,
            player->___u26.client_update_data.___u0.remote_player.action_baseline_id, vehicle_update.sequence_number,
            player->___u26.client_update_data.___u0.remote_player.last_action_update_id);
    }
}
