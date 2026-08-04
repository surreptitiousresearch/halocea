/* oddball_engine_display_score @0x838164B8 — format an Oddball score/status message string into a caller
 * buffer. Sibling of slayer_engine_display_score / king_engine_display_score. Each message pulls the
 * "ui\multiplayer_game_text" unicode string list (falling back to an empty wide string when it is not
 * loaded) and either copies a fixed string or formats it with player/score arguments:
 *   31/32/34/35 — fixed status strings (162/163/159/160)
 *   33          — "<player name>" (164)
 *   36          — "<player name>" (161)
 *   38          — "<player name>: <score>" (166)
 *   39          — "<player name>: <score>" (165)
 *   40          — "<place>: <score>" (155), place resolved via game_engine_get_place / get_place_name
 * The Oddball score shown is oddball_globals.team_score[player.team_index] / 30 (ticks → seconds).
 * Returns 1 when a string was produced, 0 otherwise.
 *
 * DEVIATIONS: the decompiler mistyped player_index as game_engine_place* — it is a plain int player index
 * (used as a datum index and passed to game_engine_get_place). game_engine_get_place returns a
 * game_engine_place BY VALUE and takes (player_index, score_type); the by-value-return ABI surfaces a
 * phantom third arg. Disasm at 0x83816700 shows `li r4, 1` → score_type = _get_score_team. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/oddball_globals.h"
#include "headers/game_engine_place.h"
#include "headers/get_score_type.h"
#include "headers/player_datum.h"
#include "headers/oddball_message.h"
#include "headers/blam_data_globals.h"
#include "headers/multiplayer_game_text_string.h"

extern const wchar_t empty_wide_string; /* empty wide string fallback */

extern void *datum_try_and_get(const data_array *data, int index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern game_engine_place game_engine_get_place(int player_index, get_score_type score_type);
extern uint16_t * get_place_name(game_engine_place place);

uint8_t oddball_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size)
{
    switch ( message )
    {
        case oddball_message_you_have_the_ball:
        case oddball_message_ally_has_the_ball:
        case oddball_message_you_are_it:
        case oddball_message_ally_is_it:
        {
            int string_index;
            if ( message == oddball_message_you_have_the_ball )       string_index = 162;
            else if ( message == oddball_message_ally_has_the_ball )  string_index = 163;
            else if ( message == oddball_message_you_are_it )         string_index = 159;
            else                                                     string_index = 160;

            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            const wchar_t *string = (text_tag != -1)
                    ? unicode_string_list_get_string(text_tag, string_index)
                    : &empty_wide_string;
            ustrncpy(buffer, string, buffer_size);
            return 1;
        }

        case oddball_message_enemy_has_the_ball:
        case oddball_message_enemy_is_it:
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !player )
                return 0;
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            if ( text_tag == -1 )
            {
                usnprintf(buffer, buffer_size, &empty_wide_string, player->name);
                return 1;
            }
            const wchar_t *format = unicode_string_list_get_string(text_tag, message == oddball_message_enemy_has_the_ball ? 164 : 161);
            usnprintf(buffer, buffer_size, format, player->name);
            return 1;
        }

        case oddball_message_enemy_has_the_ball_tick:
        case oddball_message_ally_has_the_ball_tick:
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !player )
                return 0;
            int score = oddball_globals.team_score[player->team_index] / 30;   /* team_index @0x20 */
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            if ( text_tag == -1 )
                usnprintf(buffer, buffer_size, &empty_wide_string, player->name, score);
            else
                usnprintf(buffer, buffer_size,
                        unicode_string_list_get_string(text_tag, message == oddball_message_enemy_has_the_ball_tick ? 166 : 165),
                        player->name, score);   /* player name, score */
            return 1;
        }

        case oddball_message_you_have_the_ball_tick:
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !datum_try_and_get(player_data, player_index) || !player )
                return 0;
            game_engine_place place = game_engine_get_place(player_index, _get_score_team);
            unsigned __int16 *place_name = get_place_name(place);
            int score = oddball_globals.team_score[player->team_index] / 30;
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            if ( text_tag == -1 )
                usnprintf(buffer, buffer_size, &empty_wide_string, place_name, score);
            else
                usnprintf(buffer, buffer_size, unicode_string_list_get_string(text_tag, _string_place_score_seconds),
                        place_name, score);
            return 1;
        }

        default:
            return 0;
    }
}
