/* ctf_engine_display_score @0x83806740 — format a CTF scoreboard / event message into a UI text buffer.
 * Twin of oddball_engine_display_score / slayer_engine_display_score: a switch over the message id selects a
 * string from the "ui\multiplayer_game_text" unicode string list (falling back to the empty empty_wide_string
 * when the tag is not loaded) and formats it with the two teams' capture scores. Messages 32-34 resolve the
 * requesting player's team (player[8]) and print that team's score first; 36-46 copy a plain event string.
 * Returns 1 when a message was produced, 0 for an unknown message id (or a missing player). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/ctf_globals.h"
#include "headers/player_datum.h"
#include "headers/ctf_message.h"
#include "headers/multiplayer_game_text_string.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

extern void *datum_try_and_get(const data_array *data, int index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);

uint8_t ctf_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size)
{
    int result;
    int tag_index;
    const wchar_t *string;

    /* `message` is a ctf_message id (31..46); the string index is a multiplayer_game_text_string. */
    switch ( message )
    {
        case ctf_message_score:
            tag_index = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            if ( tag_index == -1 )
                usnprintf(buffer, buffer_size, empty_wide_string, ctf_globals.score[0], ctf_globals.score[1]);
            else
            {
                string = unicode_string_list_get_string(tag_index, _string_red_team_score_blue_team_score);
                usnprintf(buffer, buffer_size, string, ctf_globals.score[0], ctf_globals.score[1]);
            }
            return 1;

        case ctf_message_you_scored:
        case ctf_message_enemy_scored:
        case ctf_message_good_guys_scored:
        {
            player_datum *player = datum_try_and_get(player_data, player_index);
            if ( !player )
                return 0;
            int team = player->team_index;
            int other_team = (team + 1) % 2;
            int string_index = _string_you_scored_score_to_score + (message - ctf_message_you_scored);
            tag_index = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            if ( tag_index == -1 )
                usnprintf(buffer, buffer_size, empty_wide_string, ctf_globals.score[team], ctf_globals.score[other_team]);
            else
            {
                string = unicode_string_list_get_string(tag_index, string_index);
                usnprintf(buffer, buffer_size, string, ctf_globals.score[team], ctf_globals.score[other_team]);
            }
            return 1;
        }

        case ctf_message_you_have_the_flag:
            usnprintf(buffer, buffer_size, empty_wide_string);
            return 1;

        case ctf_message_you_returned_the_flag:
        case ctf_message_enemy_has_the_flag:
        case ctf_message_enemy_has_the_flag_tick:
        case ctf_message_enemy_returned_the_flag:
        case ctf_message_good_guys_have_the_flag:
        case ctf_message_good_guys_returned_the_flag:
        case ctf_message_your_flag_was_returned:
        case ctf_message_enemy_flag_was_returned:
        case ctf_message_time_expired:
        case ctf_message_you_are_on_attack:
        case ctf_message_you_are_on_defense:
        {
            /* you_returned→you_returned string, {enemy_has,enemy_has_tick}→the_enemy_has_your_flag,
             * then enemy_returned..you_are_on_defense map contiguously from the_enemy_returned_the_flag */
            int string_index;
            if ( message == ctf_message_you_returned_the_flag )
                string_index = _string_you_returned_the_flag;
            else if ( message == ctf_message_enemy_has_the_flag || message == ctf_message_enemy_has_the_flag_tick )
                string_index = _string_the_enemy_has_your_flag;
            else
                string_index = _string_the_enemy_returned_the_flag + (message - ctf_message_enemy_returned_the_flag);
            tag_index = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            if ( tag_index == -1 )
                string = empty_wide_string;
            else
                string = unicode_string_list_get_string(tag_index, string_index);
            ustrncpy(buffer, string, buffer_size);
            result = 1;
            break;
        }

        default:
            result = 0;
            break;
    }
    return result;
}
