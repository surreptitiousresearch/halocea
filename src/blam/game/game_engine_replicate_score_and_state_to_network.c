/* game_engine_replicate_score_and_state_to_network @0x83752CF8 — broadcast every active player's score to
 * the network as a player_score_update message-delta, then hand off to the game mode's own state
 * replication callback. For each used score slot, the current score is gathered from the player datum;
 * juggernaut/oddball time (multiplayer stat 0) is converted from ticks to seconds. In stateless mode the
 * stored baseline is encoded; in incremental mode the freshly gathered score is encoded against the stored
 * baseline and then becomes the new baseline.
 *
 * Reconstructed against disasm: the decompiler lost the player_score_baseline_slot struct (rendering
 * slot.is_slot_used / slot.player_index as bogus negative indexing into the baseline) and packed the
 * gathered body with 64-bit store artifacts. Note the asymmetry (faithful to the binary): the stateless
 * path encodes the stored baseline, not the freshly gathered body. */

#include <stdint.h>
#include "headers/player_score_baseline_slot.h"
#include "headers/player_datum.h"
#include "headers/player_score_network_data.h"
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/game_variant.h"
#include "headers/game_engine.h"
#include "headers/network_game_server.h"
#include "headers/iterated_message.h"
#include "headers/game_engine_type.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_message_type.h"
#include "headers/blam_data_globals.h"

/* game_engine: canonical decl from game_engine.h (via blam_data_globals.h) */

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
/* _message_player_score_update / network_message_type_message_delta: immediates in the binary
 * (li r3,0x15 @0x83752DE8, li r4,1 @0x83752E44) — enum constants from the headers below, not externs. */
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void game_engine_replicate_score_and_state_to_network(message_delta_processor_mode mode, int machine_index)
{
    /* the binary bounds the loop by comparing the slot pointer against &global_goal[0]; equivalently this
     * walks all 32 score slots. */
    for (int slot_index = 0; slot_index < 32; slot_index++)
    {
        player_score_baseline_slot *slot = &g_player_score_baselines[slot_index];
        if (slot->is_slot_used != 1)
            continue;

        int player_index = slot->player_index;
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

        int source_header[3] = {0};
        source_header[0] = field_translated_index_translate_index(&field_properties_player_index_definition,
                                                                  player_index);

        player_score_network_data score;
        *(long long *)score.kills = *(long long *)player->statistics.kills;
        *(long long *)score.assists = *(long long *)player->statistics.assists;
        score.friendly_fire_kills = player->statistics.friendly_fire_kills;
        score.deaths = player->statistics.deaths;
        score.suicides = player->statistics.suicides;
        score.multiplayer_stats = player->statistics.multiplayer_statistics;
        score.special = player->multiplayer.special;
        score.is_odd_man_out = player->multiplayer.is_odd_man_out;
        score.speed_multiplier = player->multiplayer.speed_multiplier;

        if (global_variant.game_engine_index == game_engine_king) /* juggernaut/oddball: ticks -> seconds */
            score.multiplayer_stats.ctf_statistics.flag_grabs /= 30;

        int size_in_bits;
        if (mode)
            size_in_bits = message_delta_processor_encode_incremental(
                _message_player_score_update, source_header, &score, &slot->baseline,
                g_message_encode_buffer, 32760, 0);
        else
            size_in_bits = message_delta_processor_encode_stateless(
                _message_player_score_update, source_header, &slot->baseline,
                g_message_encode_buffer, 32760);

        if (size_in_bits > 0)
        {
            network_game_server *server = global_network_game_server_get();
            network_game_server_send_message_to_all_loaded_machines(
                server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits,
                1u, 0, 0, 3);
        }

        if (mode == _message_delta_mode_incremental)
            slot->baseline = score; /* the freshly gathered score becomes the new baseline */
    }

    if (game_engine->replicate_game_mode_state_to_network)
        game_engine->replicate_game_mode_state_to_network(mode, machine_index);
}
