/* game_engine_player_killed @0x83753298 — central multiplayer death handler. Records the death, applies
 * the respawn-timer / penalty economy (base respawn time, escalating growth penalty, suicide and
 * friendly-fire penalties, the growth "refund" the killer earns), fires the game_engine's
 * player_killed_player callback, replicates score+state to the network, and finally broadcasts the death
 * to every player's HUD (score banners plus the localized kill/betrayal chat lines and multikill /
 * killing-spree medals). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/object_header_datum.h"
#include "headers/object_type.h"
#include "headers/game_connection.h"
#include "headers/game_engine_message.h"
#include "headers/blam_data_globals.h"


#include "headers/network_game_server.h"
#include "headers/message_delta_message_ids.h"
extern int game_time_get(void);
extern int16_t game_connection(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void *datum_try_and_get(const data_array *data, int index);
extern void hud_chat_box_add_message(const wchar_t *message);
extern uint8_t multiplayer_message_internal(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);
extern void game_engine_replicate_score_and_state_to_network(message_delta_processor_mode mode, int machine_index);
extern void game_show_score_all_extended_except_player(int excepted_player, int subject_player_index, int message, int message_data, uint8_t should_replicate);
extern void game_show_score_extended(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

/* message-delta field descriptor — only its address is taken, so an opaque forward declaration suffices */
typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);

/* Formats the localized HUD line for `recipient_player_index` (or, when it is -1, for every in-use
 * player), pushes it to the local chat box, and on a server replicates the HUD message across the
 * network. Collapses the four copies the compiler inlined at the call sites — they differ only in the
 * message id (game_engine_message_killed_enemy = "you killed X",
 * game_engine_message_killed_friendly = "you betrayed X"). */
static void announce_kill_hud_message(int recipient_player_index, int message, int dead_player_index)
{
    wchar_t text[1024];
    int delta[3]; /* stateless HUD-message payload: {translated player index, message, message_data} */

    if (recipient_player_index == -1)
    {
        data_iterator it;
        data_iterator_new(&it, player_data);
        while (data_iterator_next(&it))
        {
            int index = it.index;
            player_datum *player = datum_try_and_get(player_data, index);
            if (!player)
                continue;

            /* local_player_index; -1 (0xFFFF) => not a local player, no local chat line */
            if ((unsigned __int16)player->local_player_index != 0xFFFF
                && multiplayer_message_internal(index, message, dead_player_index, text, 1024))
            {
                text[1023] = 0;
                hud_chat_box_add_message(text);
            }

            if (game_connection() == _game_connection_network_server)
            {
                delta[0] = field_translated_index_translate_index(&field_properties_player_index_definition, index);
                delta[1] = message;
                delta[2] = dead_player_index;
                if (message_delta_processor_encode_stateless(_message_multiplayer_hud_message, (void *)0, delta,
                                                             g_message_encode_buffer, 32760) > 0)
                    global_network_game_server_get(); /* DEVIATION: return discarded — release build stripped the server-send that consumed it */
            }
        }
        return;
    }

    {
        player_datum *player = datum_try_and_get(player_data, recipient_player_index);
        if (!player)
            return;

        if ((unsigned __int16)player->local_player_index != 0xFFFF
            && multiplayer_message_internal(recipient_player_index, message, dead_player_index,
                                            text, 1024))
        {
            text[1023] = 0;
            hud_chat_box_add_message(text);
        }

        if (game_connection() == _game_connection_network_server)
        {
            delta[0] = field_translated_index_translate_index(&field_properties_player_index_definition,
                                                              recipient_player_index);
            delta[1] = message;
            delta[2] = dead_player_index;
            if (message_delta_processor_encode_stateless(_message_multiplayer_hud_message, (void *)0, delta,
                                                         g_message_encode_buffer, 32760) > 0)
                global_network_game_server_get(); /* DEVIATION: return discarded (see above) */
        }
    }
}

void game_engine_player_killed(int killing_player_index, int killing_object_index, int dead_player_index,
                               uint8_t friendly_fire)
{
    player_datum *dead;
    int both_players_valid;
    int counts_as_kill;
    int death_message;

    dead = DATA_ARRAY_ELEMENT(player_data, player_datum, dead_player_index);
    if (!game_engine)
        return;

    dead->multiplayer.time_of_death = game_time_get();

    if (game_engine->player_killed_player)
        game_engine->player_killed_player(killing_player_index, killing_object_index, dead_player_index,
                                          friendly_fire);

    both_players_valid = (killing_player_index != -1 && dead_player_index != -1);
    /* a legitimate enemy kill: not friendly fire, both players real, and not a suicide */
    counts_as_kill = (!friendly_fire && both_players_valid && dead_player_index != killing_player_index);

    /* respawn timer = accumulated growth penalty + base respawn time */
    dead->respawn_timer = dead->respawn_penalty + global_variant.universal_variant.respawn_time;

    if (global_variant.universal_variant.respawn_time_growth > 0)
    {
        int grown = dead->respawn_penalty + global_variant.universal_variant.respawn_time_growth;
        if (grown > 5 * global_variant.universal_variant.respawn_time_growth)
            grown = 5 * global_variant.universal_variant.respawn_time_growth;
        dead->respawn_penalty = grown;

        if (counts_as_kill && killing_player_index != -1)
        {
            /* the killer earns one growth step back off their own penalty, clamped at zero */
            player_datum *killer = DATA_ARRAY_ELEMENT(player_data, player_datum, killing_player_index);
            int refunded = killer->respawn_penalty - global_variant.universal_variant.respawn_time_growth;
            killer->respawn_penalty = (refunded < 0) ? 0 : refunded; /* max(0,x) — see cntlzw/carry idiom in disasm */
        }
    }

    if (!counts_as_kill)
        dead->respawn_timer += global_variant.universal_variant.suicide_penalty;

    if (dead->statistics.killed_teammate_since_last_death)
    {
        dead->respawn_timer += global_variant.universal_variant.friendly_fire_penalty;
        dead->statistics.killed_teammate_since_last_death = 0;
    }

    if (dead->respawn_timer <= 90)
        dead->respawn_timer = 90;

    if (game_connection() == _game_connection_network_server)
        game_engine_replicate_score_and_state_to_network(_message_delta_mode_incremental, -1);

    if (dead->quit_out_of_game)
        return;

    /* classify the death for the score banners */
    if (killing_player_index == -1)
    {
        death_message = game_engine_message_killed_by_unknown; /* generic / environment death */
        if (killing_object_index != -1)
        {
            object_datum *killer_object =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, killing_object_index)->datum;
            object_try_and_get_and_verify_type(killing_object_index, object_mask_unit); /* validation only; return discarded */

            /* object.type: 0 = biped, 1 = vehicle, else default death message */
            if (!killer_object->object.type)
                death_message = game_engine_message_killed_by_biped;
            else if (killer_object->object.type == object_type_vehicle)
                death_message = game_engine_message_killed_by_vehicle;
        }
    }
    else
    {
        if (killing_player_index == dead_player_index)
        {
            /* suicide: broadcast to everyone but the dead player, no further per-killer messaging */
            game_show_score_all_extended_except_player(-1, dead_player_index,
                                                       game_engine_message_killed_by_self,
                                                       killing_player_index, 1u);
            return;
        }
        /* enemy kill: killed_by_player (4) clean, killed_by_friendly_fire (5) betrayal
         * (cntlzw idiom: friendly_fire ? 5 : 4) */
        death_message = (friendly_fire != 0) + game_engine_message_killed_by_player;
    }

    game_show_score_all_extended_except_player(killing_player_index, dead_player_index, death_message,
                                               killing_player_index, 1u);

    if (death_message == game_engine_message_killed_by_friendly_fire)
    {
        /* friendly fire: tell the killer they betrayed a teammate */
        announce_kill_hud_message(killing_player_index, game_engine_message_killed_friendly, dead_player_index);
        return;
    }
    if (death_message != game_engine_message_killed_by_player)
        return;

    /* clean enemy kill: award multikill / killing-spree medals off the killer's statistics */
    {
        player_datum *killer = DATA_ARRAY_ELEMENT(player_data, player_datum, killing_player_index);
        int spree_message = 0;

        if (killer->statistics.multiple_kills >= 4)
            spree_message = game_engine_message_multi_kill;
        else if (killer->statistics.multiple_kills == 3)
            spree_message = game_engine_message_triple_kill;
        else if (killer->statistics.multiple_kills == 2)
            spree_message = game_engine_message_double_kill;
        else
        {
            int streak = killer->statistics.kills_in_a_row;
            if (streak == 5)
                spree_message = game_engine_message_five_kills_in_row;
            else if (streak == 5 * (streak / 5)) /* other multiple of 5 */
                spree_message = game_engine_message_ten_kills_in_a_row;
        }

        if (spree_message)
            game_show_score_extended(killing_player_index, killing_player_index, spree_message,
                                     dead_player_index, 1u);
    }

    announce_kill_hud_message(killing_player_index, game_engine_message_killed_enemy, dead_player_index);
}
