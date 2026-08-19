/* player_update_client_remote_player_action_update_from_network @0x8376E2D0 — (client) apply a replicated
 * remote player's action update: zero the decode scratch, translate the batch header's network player
 * index to a local one in place, and if that player datum exists, decode either incrementally (against
 * the player's cached baseline at datum +256) or statelessly, handing a successful decode to
 * handle_remote_player_action_update. If the player doesn't exist locally, just discards the iteration.
 *
 * DEVIATION: the decompiler shows 4 phantom trailing parameters (a3..a6); DB prototype confirms only
 * header/client are real — a6's "zero" literal is just the value used to memset the local scratch buffer. */

#include <stdint.h>
#include <string.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/remote_player_action_update_header.h"
#include "headers/remote_player_action_update_network_data.h"
#include "headers/player_datum.h"
#include "headers/field_properties_definition.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_incremental(void *const destination_data, const void *const baseline_data, const message_delta_processor_header *const header, const uint8_t allow_empty_body);
extern void handle_remote_player_action_update(remote_player_action_update_header *player_update_header, remote_player_action_update_network_data *update_data, uint8_t is_stateless);
/* memcpy declared by <string.h> */

void player_update_client_remote_player_action_update_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    remote_player_action_update_network_data network_data;
    memset(&network_data, 0, sizeof(network_data));

    remote_player_action_update_header *custom_header = (remote_player_action_update_header *)header->custom_header;
    int local_player_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
                                  custom_header->player_index);
    custom_header->player_index = local_player_index;

    player_datum *player = datum_try_and_get(player_data, local_player_index);
    if ( !player )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    if ( header->decoding_information->mode )
    {
        /* recovered: player + 256 -> &player->___u26.client_update_data.___u0.remote_player.action_update_baseline */
        const remote_player_action_update_network_data *baseline =
            &player->___u26.client_update_data.___u0.remote_player.action_update_baseline;
        memcpy(&network_data, baseline, sizeof(network_data));
        if ( message_delta_processor_decode_incremental(&network_data, baseline, header, 1u) == 1 )
            handle_remote_player_action_update(custom_header, &network_data, 0);
    }
    else if ( message_delta_processor_decode_stateless(&network_data, header) == 1 )
    {
        handle_remote_player_action_update(custom_header, &network_data, 1u);
    }
}
