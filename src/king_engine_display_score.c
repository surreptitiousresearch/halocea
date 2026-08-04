/* king_engine_display_score @0x8382C720 — format a King-of-the-Hill score message string into a caller buffer.
 * Message 33 prints "<place>: <score>" for the querying player's standing (via game_engine_get_place /
 * get_place_name); messages 31 and 32 print a player's own score with the "you control"/"controlling" template
 * (string 157 vs 156). Scores are king_globals.score in ticks, shown in seconds (/30). Each pulls the
 * "ui\multiplayer_game_text" unicode string list, falling back to an empty string when it is not loaded.
 *
 * game_engine_get_place returns a game_engine_place BY VALUE (2 args; disasm 0x8382C78C sets only r3/r4) and
 * that value is passed straight into get_place_name, which also takes game_engine_place by value — the r3
 * return of game_engine_get_place is the r3 argument to get_place_name with no intervening store (disasm
 * 0x8382C794-0x8382C798). Messages 31 and 32 share identical logic apart from the template index, so their
 * duplicated Hex-Rays cases are collapsed. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/king_globals.h"
#include "headers/game_engine_place.h"
#include "headers/get_score_type.h"
#include "headers/player_datum.h"
#include "headers/king_message.h"
#include "headers/blam_data_globals.h"
#include "headers/multiplayer_game_text_string.h"

extern const wchar_t empty_wide_string; /* empty wide string fallback */

extern void *datum_try_and_get(const data_array *data, int index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern game_engine_place game_engine_get_place(int player_index, get_score_type score_type);
extern uint16_t * get_place_name(game_engine_place place);

uint8_t king_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size)
{
    switch ( message )
    {
        case king_message_enemy_on_the_hill:
        case king_message_ally_on_the_hill:
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !player )
                return 0;
            int score = king_globals.score[player->team_index] / 30;
            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            const wchar_t *format = (text_tag != -1)
                ? unicode_string_list_get_string(text_tag, message == king_message_enemy_on_the_hill ? 157 : 156)
                : &empty_wide_string;
            usnprintf(buffer, buffer_size, format, player->name, score);
            return 1;
        }

        case king_message_you_are_on_the_hill:
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( datum_try_and_get(player_data, player_index) && player )
            {
                game_engine_place place = game_engine_get_place(player_index, _get_score_team);
                unsigned __int16 *place_name = get_place_name(place);
                int score = king_globals.score[player->team_index] / 30;
                int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
                if ( text_tag == -1 )
                    usnprintf(buffer, buffer_size, &empty_wide_string, place_name, score);
                else
                    usnprintf(buffer, buffer_size, unicode_string_list_get_string(text_tag, _string_place_score_seconds), place_name, score);
                return 1;
            }
            break;
        }
    }
    return 0;
}
