/* player_update_client_super_remote_players_action_update_from_network @0x8376E3C8 — (client) apply one
 * player's action update from a "super" (multi-player) batch: translate the batch header's network player
 * index to a local one in place. For an incremental update, stage the decode baseline from that specific
 * player's own cached network baseline (player datum +256, 48 bytes) — but only when the player exists
 * locally and isn't itself a local player (local_player_index word +2 == 0xFFFF); otherwise the staging
 * buffer is left as whatever the stack happened to hold, which is harmless since the final apply below is
 * gated on the same "not local" check. For a stateless update, the decode target is simply zeroed first.
 * After a successful decode, re-resolves the player datum by the (now-translated) header index and applies
 * the update via handle_remote_player_action_update only if that player is confirmed not local. Sibling of
 * player_update_client_remote_player_action_update_from_network.c (the single-player variant), which
 * always seeds from a fixed baseline location rather than per-player. */

#include <stdint.h>
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
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_incremental(void *destination_data, const void *baseline_data,
        const message_delta_processor_header *header, uint8_t allow_empty_body);
extern void handle_remote_player_action_update(remote_player_action_update_header *player_update_header, remote_player_action_update_network_data *update_data, uint8_t is_stateless);
extern void *memset(void *destination, int value, unsigned int size);
extern void *memcpy(void *destination, const void *source, unsigned int size);

void player_update_client_super_remote_players_action_update_from_network(message_delta_processor_header *header,
        network_game_client *client)
{
    remote_player_action_update_header *custom_header = (remote_player_action_update_header *)header->custom_header;
    int local_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
                           custom_header->player_index);
    custom_header->player_index = local_index;

    remote_player_action_update_network_data network_data;
    remote_player_action_update_network_data baseline;   /* left unseeded when not applicable — harmless, see above */
    uint8_t is_stateless;
    uint8_t decoded;

    if ( header->decoding_information->mode )
    {
        player_datum *player = datum_try_and_get(player_data, local_index);
        if ( player && (uint16_t)player->local_player_index == 0xFFFF )
            /* recovered: player + 256 -> &player->___u26.client_update_data.___u0.remote_player.action_update_baseline */
            memcpy(&baseline, &player->___u26.client_update_data.___u0.remote_player.action_update_baseline, sizeof(baseline));

        memcpy(&network_data, &baseline, sizeof(network_data));
        is_stateless = 0;
        decoded = message_delta_processor_decode_incremental(&network_data, &baseline, header, 1u);
    }
    else
    {
        memset(&network_data, 0, sizeof(network_data));
        is_stateless = 1;
        decoded = message_delta_processor_decode_stateless(&network_data, header);
    }

    if ( decoded == 1 )
    {
        player_datum *applied_player = datum_try_and_get(player_data, custom_header->player_index);
        if ( applied_player && (uint16_t)applied_player->local_player_index == 0xFFFF )
            handle_remote_player_action_update(custom_header, &network_data, is_stateless);
    }
}
