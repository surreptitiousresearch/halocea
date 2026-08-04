/* game_engine_player_score_replicate_from_network @0x8374FFF0 — client-side decode counterpart of
 * game_engine_replicate_player_score_to_network.c: apply an incoming player_score_update message-delta to the
 * addressed player's score baseline slot, then push the decoded fields into the live player_datum. Resolves
 * the target slot from the message's translated player index; discards the iteration body if the player has
 * no score slot. Incremental updates decode into a copy seeded from the current baseline and write the
 * result back; stateless updates decode straight into the baseline. On a successful decode, the baseline's
 * kills/assists/friendly_fire/deaths/suicides/multiplayer-stats/special/is_odd_man_out/speed_multiplier are
 * copied into the player_datum, with the oddball clock (multiplayer_statistics.ctf_statistics.flag_grabs) scaled back up by 30
 * (the inverse of the to_network /30) when the variant is oddball (game_engine_index == game_engine_king).
 *
 * DEVIATION: the decompiler rendered the seed/write-back of the 40-byte player_score_network_data as two
 * 10-iteration walking-pointer copy loops (`v8 += 4; *(_DWORD*)v8 = *++p`); restored to plain struct
 * assignments (10 ints * 4 = sizeof(player_score_network_data)). The `client` parameter is unused, matching
 * the DB prototype. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/player_score_baseline_slot.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/game_variant.h"
#include "headers/game_engine_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern int game_engine_player_score_get_slot_index_for_player(int player_index);
extern unsigned __int8 message_delta_processor_decode_incremental(void *destination_data, const void *baseline_data,
    const message_delta_processor_header *header, unsigned __int8 allow_empty_body);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern void *datum_try_and_get(const data_array *data, int index);

void game_engine_player_score_replicate_from_network(message_delta_processor_header *header,
    network_game_client *client)
{
    int local_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
        *(int *)header->custom_header);
    int slot = game_engine_player_score_get_slot_index_for_player(local_index);

    if ( slot == -1 )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    player_score_network_data *baseline = &g_player_score_baselines[slot].baseline;
    unsigned __int8 decoded;

    if ( header->decoding_information->mode )
    {
        player_score_network_data decoded_state = *baseline;
        decoded = message_delta_processor_decode_incremental(&decoded_state, baseline, header, 0);
        *baseline = decoded_state;
    }
    else
    {
        decoded = message_delta_processor_decode_stateless(baseline, header);
    }

    if ( decoded != 1 )
        return;

    player_datum *player = datum_try_and_get(player_data, g_player_score_baselines[slot].player_index);
    if ( !player )
        return;

    player->statistics.kills[0] = baseline->kills[0];
    player->statistics.kills[1] = baseline->kills[1];
    player->statistics.kills[2] = baseline->kills[2];
    player->statistics.kills[3] = baseline->kills[3];
    player->statistics.assists[0] = baseline->assists[0];
    player->statistics.assists[1] = baseline->assists[1];
    player->statistics.assists[2] = baseline->assists[2];
    player->statistics.assists[3] = baseline->assists[3];
    player->statistics.friendly_fire_kills = baseline->friendly_fire_kills;
    player->statistics.deaths = baseline->deaths;
    player->statistics.suicides = baseline->suicides;
    player->statistics.multiplayer_statistics.ctf_statistics.flag_grabs = baseline->multiplayer_stats.ctf_statistics.flag_grabs;
    player->statistics.multiplayer_statistics.ctf_statistics.flag_returns = baseline->multiplayer_stats.ctf_statistics.flag_returns;
    player->statistics.multiplayer_statistics.ctf_statistics.flag_scores = baseline->multiplayer_stats.ctf_statistics.flag_scores;
    player->multiplayer.special = baseline->special;
    player->multiplayer.is_odd_man_out = baseline->is_odd_man_out;
    player->multiplayer.speed_multiplier = baseline->speed_multiplier;

    if ( global_variant.game_engine_index == game_engine_king )
        player->statistics.multiplayer_statistics.ctf_statistics.flag_grabs = 30 * baseline->multiplayer_stats.ctf_statistics.flag_grabs;
}
