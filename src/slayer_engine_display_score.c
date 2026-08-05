/* slayer_engine_display_score @0x83815B00 — format a Slayer score message string into a caller buffer.
 * Message 22 prints the querying player's standing: in a team variant "<place>: <individual> / <team> / <goal>"
 * (string 181), otherwise "<place>: <team> / <goal>" (string 182); the place is resolved via
 * game_engine_get_place / get_place_name. Message 31 prints a player's name (string 180). Each pulls the
 * "ui\multiplayer_game_text" unicode string list, falling back to an empty string when it is not loaded.
 *
 * game_engine_get_place returns a game_engine_place BY VALUE and takes (player_index, score_type); the
 * decompiler renders this as a 3-arg call with a phantom r5 local (by-value-return ABI). Disasm at 0x83815BD4
 * shows `li r4, 1` dominating BOTH the teams and the individual call sites (it precedes the branch), so both
 * pass score_type = _get_score_team. The individual branch reads team_score (not individual_score) — faithful. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/slayer_globals.h"
#include "headers/game_engine_place.h"
#include "headers/get_score_type.h"
#include "headers/game_variant.h"
#include "headers/game_engine_message.h"
#include "headers/slayer_message.h"
#include "headers/multiplayer_game_text_string.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string; /* empty wide string fallback */

extern void *datum_try_and_get(const data_array *data, int index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern game_engine_place game_engine_get_place(int player_index, get_score_type score_type);
extern uint16_t * get_place_name(game_engine_place place);
extern game_variant *game_engine_get_variant(void);

uint8_t slayer_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size)
{
    if ( message != game_engine_message_show_score ) /* 22 */
    {
        if ( message == slayer_message_new_target ) /* 31 */
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( player )
            {
                int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
                const wchar_t *format = (text_tag != -1)
                    ? unicode_string_list_get_string(text_tag, _string_new_target_name)
                    : &empty_wide_string;
                usnprintf(buffer, buffer_size, format, player->name);
                return 1;
            }
        }
        return 0;
    }

    if ( !datum_try_and_get(player_data, player_index) )
        return 0;

    if ( game_engine_get_variant()->universal_variant.teams )
    {
        game_engine_place place = game_engine_get_place(player_index, _get_score_team);
        uint16_t *place_name = get_place_name(place);
        int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
        const wchar_t *format = (text_tag != -1)
            ? unicode_string_list_get_string(text_tag, _string_name_kills_score_n_team_score_of_max)
            : &empty_wide_string;
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int individual_score = slayer_globals.individual_score[player_index];
        int team_score = slayer_globals.team_score[player->team_index];
        usnprintf(buffer, buffer_size, format, place_name, individual_score, team_score,
                  game_engine_get_variant()->universal_variant.score_to_win);
        return 1;
    }
    else
    {
        game_engine_place place = game_engine_get_place(player_index, _get_score_team);
        uint16_t *place_name = get_place_name(place);
        int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
        const wchar_t *format = (text_tag != -1)
            ? unicode_string_list_get_string(text_tag, _string_name_kills_score_of_max)
            : &empty_wide_string;
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int team_score = slayer_globals.team_score[player->team_index];
        usnprintf(buffer, buffer_size, format, place_name, team_score,
                  game_engine_get_variant()->universal_variant.score_to_win);
        return 1;
    }
}
