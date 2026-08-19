/* game_engine_replicate_player_score_to_network @0x837501A8 — gather a player's live score fields
 * (game_statistics.kills/assists/friendly_fire_kills/deaths/suicides/multiplayer_statistics,
 * multiplayer_player_info.special/is_odd_man_out, multiplayer_player_info.speed_multiplier) into a
 * player_score_network_data payload, converting the oddball/juggernaut clock (multiplayer_stats.ctf_statistics.flag_grabs)
 * from ticks to seconds when the variant is oddball (game_engine_index == game_engine_king), then encode+send a
 * player_score_update message-delta exactly like game_engine_player_score_encode_and_send_score_update.c's
 * standalone sibling (incremental against the baseline, or stateless), and — for an incremental encode —
 * copy the freshly-gathered payload back into the baseline slot for next time.
 *
 * DEVIATION: the decompiler renders the two consecutive 8-byte field copies (kills[4]+assists[4], read via
 * `ld`/`std` as raw 64-bit loads since they're naturally aligned) as `__int64` locals whose upper halves it
 * "resolves" to two utterly unrelated global addresses (`&weather_particle_system_globals...`,
 * `&global_variant`) — pure decompiler noise from coincidentally matching bit patterns, not real
 * assignments; disasm confirms both are simple 8-byte struct-field copies. The trailing
 * baseline-copy-back loop (`v13 += 4; *++p_speed_multiplier = ...`, 10 iterations) is the same
 * whole-struct walking-pointer idiom seen elsewhere this session — restored to a plain struct assignment
 * (10 ints * 4 bytes = sizeof(player_score_network_data), confirmed). */

#include <stdint.h>
#include "headers/message_delta_processor_mode.h"
#include "headers/player_score_baseline_slot.h"
#include "headers/player_score_update_header.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/game_variant.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/message_delta_message_ids.h"
#include "headers/game_engine_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void game_engine_replicate_player_score_to_network(int slot, message_delta_processor_mode mode,
    const int machine_index)
{
    int player_index = g_player_score_baselines[slot].player_index;
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    player_score_update_header header;
    header.player_index = field_translated_index_translate_index(&field_properties_player_index_definition,
        player_index);

    player_score_network_data current_state;
    current_state.kills[0] = player->statistics.kills[0];
    current_state.kills[1] = player->statistics.kills[1];
    current_state.kills[2] = player->statistics.kills[2];
    current_state.kills[3] = player->statistics.kills[3];
    current_state.assists[0] = player->statistics.assists[0];
    current_state.assists[1] = player->statistics.assists[1];
    current_state.assists[2] = player->statistics.assists[2];
    current_state.assists[3] = player->statistics.assists[3];
    current_state.friendly_fire_kills = player->statistics.friendly_fire_kills;
    current_state.deaths = player->statistics.deaths;
    current_state.suicides = player->statistics.suicides;
    current_state.multiplayer_stats.ctf_statistics.flag_grabs = player->statistics.multiplayer_statistics.ctf_statistics.flag_grabs;
    current_state.multiplayer_stats.ctf_statistics.flag_returns = player->statistics.multiplayer_statistics.ctf_statistics.flag_returns;
    current_state.multiplayer_stats.ctf_statistics.flag_scores = player->statistics.multiplayer_statistics.ctf_statistics.flag_scores;
    current_state.special = player->multiplayer.special;
    current_state.is_odd_man_out = player->multiplayer.is_odd_man_out;
    current_state.speed_multiplier = player->multiplayer.speed_multiplier;

    if ( global_variant.game_engine_index == game_engine_king )
        current_state.multiplayer_stats.ctf_statistics.flag_grabs /= 30;

    player_score_network_data *baseline = &g_player_score_baselines[slot].baseline;

    int encoded_size_in_bits;
    if ( mode )
        encoded_size_in_bits = message_delta_processor_encode_incremental(_message_player_score_update, &header,
            &current_state, baseline, g_message_encode_buffer, 32760, 0);
    else
        encoded_size_in_bits = message_delta_processor_encode_stateless(_message_player_score_update, &header,
            baseline, g_message_encode_buffer, 32760);

    if ( encoded_size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        if ( machine_index == -1 )
            network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
                g_message_encode_buffer, encoded_size_in_bits, 1u, 0, 0, 3);
        else
            network_game_server_send_message_to_machine(server, machine_index, network_message_type_message_delta,
                g_message_encode_buffer, encoded_size_in_bits, 1u, 0, 0, 3);
    }

    if ( mode == _message_delta_mode_incremental )
        *baseline = current_state;
}
