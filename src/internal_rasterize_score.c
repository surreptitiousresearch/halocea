/* internal_rasterize_score @0x8374C178 */
#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/game_engine.h"
#include "headers/game_engine_message.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"

/* NOTE: the Blam engine symbols below (player datum array, tag + unicode-string
   lookup, wide-string formatters, multiplayer-sound and time-string helpers) are
   existing sibling/boundary functions, extern-declared here. `buffer` is the
   DB's unsigned __int16* wide-char output buffer; player names live at datum+4. */
#include "headers/data_array.h"
extern void *datum_try_and_get(const data_array *data, int index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void ustrncat(wchar_t *dest, const wchar_t *source, unsigned int count);
extern unsigned int ustrlen(const wchar_t *string);
extern void ticks_to_unicode_time_string(int ticks, unsigned int count, uint16_t *buffer);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

/* Fetch entry `string_index` from the multiplayer game-text unicode string list;
   if the tag is not loaded, fall back to the empty-string sentinel. */
static const wchar_t *get_game_text(int string_index)
{
    int tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    return (tag == -1) ? empty_wide_string : unicode_string_list_get_string(tag, string_index);
}

/* Format a multiplayer scoreboard/announcement message (identified by `message`)
   into `buffer`. Returns 1 on success, 0 if a referenced player is missing or the
   message id is unknown. */
int internal_rasterize_score(int player_index, int message, int message_data,
                             uint16_t *buffer, unsigned int buffer_size)
{
    int result = 1;

    /* in team games, remap the individual-score messages (7..12) to their team
       variants */
    if (game_engine && game_engine->game_engine_test_flag
        && game_engine->game_engine_test_flag(1)
        && (unsigned int)(message - game_engine_message_double_kill) <= 5)
    {
        /* team games: remap the individual-kill messages to their with-score variants */
        switch (message)
        {
            case game_engine_message_double_kill:      message = game_engine_message_double_kill_with_score; break;
            case game_engine_message_killed_enemy:     message = game_engine_message_killed_enemy_with_score; break;
            case game_engine_message_triple_kill:      message = game_engine_message_triple_kill_with_score; break;
            case game_engine_message_multi_kill:       message = game_engine_message_multi_kill_with_score; break;
            case game_engine_message_five_kills_in_row: message = game_engine_message_five_kills_in_row_with_score; break;
            default: message = game_engine_message_ten_kills_in_a_row_with_score; break; /* ten_kills_in_a_row */
        }
    }

    switch (message)
    {
        /* ---- single player name ---- */
        case game_engine_message_welcome:
        case game_engine_message_killed_by_unknown:
        case game_engine_message_killed_by_biped:
        case game_engine_message_killed_by_vehicle:
        case game_engine_message_killed_by_self:
        {
            static const short strings[] = { 74, 75, 76, 77, 0, 0, 81 };
            player_datum *player = datum_try_and_get(player_data, player_index);
            if (!player) { result = 0; break; }
            usnprintf(buffer, buffer_size, get_game_text(strings[message]), player->name);
            break;
        }
        case game_engine_message_killed_friendly:
        case game_engine_message_quit:
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if (!player) { result = 0; break; }
            usnprintf(buffer, buffer_size,
                      get_game_text(message == game_engine_message_killed_friendly ? 82 : 80), player->name);
            break;
        }
        case game_engine_message_killed_enemy:
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if (!player) { result = 0; break; }
            usnprintf(buffer, buffer_size, get_game_text(88), player->name);
            break;
        }

        /* ---- two player names ---- */
        case game_engine_message_killed_by_player:
        case game_engine_message_killed_by_friendly_fire:
        {
            player_datum *other = datum_try_and_get(player_data, message_data);
            player_datum *player = datum_try_and_get(player_data, player_index);
            if (!player || !other) { result = 0; break; }
            usnprintf(buffer, buffer_size,
                      get_game_text(message == game_engine_message_killed_by_player ? 78 : 79),
                      player->name, other->name);
            break;
        }

        /* ---- fixed announcement string + sound ---- */
        case game_engine_message_double_kill:
            ustrncpy(buffer, get_game_text(85), buffer_size);
            game_engine_play_multiplayer_sound(_multiplayer_sound_double_kill, 0); break;
        case game_engine_message_triple_kill:
            ustrncpy(buffer, get_game_text(84), buffer_size);
            game_engine_play_multiplayer_sound(_multiplayer_sound_triple_kill, 0); break;
        case game_engine_message_multi_kill:
            ustrncpy(buffer, get_game_text(83), buffer_size);
            game_engine_play_multiplayer_sound(_multiplayer_sound_killtacular_kill, 0); break;
        case game_engine_message_five_kills_in_row:
            ustrncpy(buffer, get_game_text(87), buffer_size);
            game_engine_play_multiplayer_sound(_multiplayer_sound_killing_spree, 0); break;
        case game_engine_message_ten_kills_in_a_row:
            ustrncpy(buffer, get_game_text(86), buffer_size);
            game_engine_play_multiplayer_sound(_multiplayer_sound_running_riot, 0); break;

        /* ---- own score + sound ---- */
        case game_engine_message_multi_kill_with_score:
        case game_engine_message_triple_kill_with_score:
        case game_engine_message_double_kill_with_score:
        case game_engine_message_ten_kills_in_a_row_with_score:
        case game_engine_message_five_kills_in_row_with_score:
        {
            static const short strings[] = { 89, 90, 91, 92, 93 };
            static const short sounds[]  = {
                _multiplayer_sound_killtacular_kill, _multiplayer_sound_triple_kill,
                _multiplayer_sound_double_kill, _multiplayer_sound_running_riot,
                _multiplayer_sound_killing_spree };
            int score;
            if (!datum_try_and_get(player_data, player_index)) { result = 0; break; }
            score = game_engine->get_score(player_index, _get_score_team);
            usnprintf(buffer, buffer_size,
                      get_game_text(strings[message - game_engine_message_multi_kill_with_score]), score);
            game_engine_play_multiplayer_sound(sounds[message - game_engine_message_multi_kill_with_score], 0);
            break;
        }

        /* ---- other player name + own score ---- */
        case game_engine_message_killed_enemy_with_score:
        {
            player_datum *other = datum_try_and_get(player_data, message_data);
            int score;
            if (!datum_try_and_get(player_data, player_index) || !other) { result = 0; break; }
            score = game_engine->get_score(player_index, _get_score_team);
            usnprintf(buffer, buffer_size, get_game_text(94), other->name, score);
            break;
        }

        /* ---- plain announcement strings ---- */
        case game_engine_message_odd_man_out:              ustrncpy(buffer, get_game_text(95), buffer_size); break;
        case game_engine_message_out_of_lives:             ustrncpy(buffer, get_game_text(96), buffer_size); break;
        case game_engine_message_waiting_for_space_to_clear: ustrncpy(buffer, get_game_text(98), buffer_size); break;
        case game_engine_message_player_quit_self:         ustrncpy(buffer, get_game_text(99), buffer_size); break;

        /* ---- numeric message ---- */
        case game_engine_message_respawn_timer:
            usnprintf(buffer, buffer_size, get_game_text(97), message_data);
            break;

        case game_engine_message_press_back_for_score:
            break;

        /* ---- elapsed-time string with a suffix ---- */
        case game_engine_message_time_left:
        {
            const wchar_t *suffix = get_game_text(189);
            unsigned int length;
            ticks_to_unicode_time_string(message_data, buffer_size, buffer);
            length = ustrlen(buffer);
            ustrncat(buffer, suffix, buffer_size - length);
            break;
        }

        default:
            result = 0;
            break;
    }

    buffer[buffer_size - 1] = 0;
    return result;
}
