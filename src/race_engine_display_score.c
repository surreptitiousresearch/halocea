/* race_engine_display_score @0x8382A888 — format a Race score/status message string into a caller buffer.
 * Sibling of oddball_engine_display_score / slayer_engine_display_score. Each message pulls the
 * "ui\multiplayer_game_text" unicode string list (falling back to an empty wide string when it is not loaded)
 * and either copies a fixed string or formats it with player / lap / place / time arguments:
 *   22 — "<place>" / "<place> (lap N)" / "<place> won" style, chosen by whether the mode is lap-race
 *        (game_engine_variant.terminator.ignored == 2) or a score-race, and by the player's lap count
 *        (subject[99]) vs universal_variant.score_to_win. The place is resolved via game_engine_get_place /
 *        get_place_name.
 *   31 — "<lap count>  <lap time>" (170); lap time = subject[98] ticks / 30.
 *   32/33 — "<player name>: <score>" (171 / 172)
 *   34 — a fixed status string (167)
 *   35/36 — "<player name>" (168 / 169)
 *   37 — "<best lap time>" (173); time = subject[100] ticks / 30.
 * Returns 1 when a string was produced, 0 otherwise.
 *
 * DEVIATIONS (disasm-verified 0x8382A8CC-0x8382AE34): player_index is a plain int (the decompiler mistyped it
 * game_engine_place*). game_engine_get_place returns game_engine_place BY VALUE and takes (player_index,
 * score_type); the by-value-return ABI surfaces a phantom third arg; score_type is _get_score_team (1). The
 * two time-formatting cases were mangled by float-in-varargs (FPR shadow): case 31's `LODWORD(v20),v20` is a
 * single %f (0x8382AAF8: r7=double bits, f1=float); case 37's format was dropped entirely — disasm 0x8382AC28
 * shows the format IS unicode_string_list_get_string(tag,173) (empty-wide fallback), and the only vararg is the
 * float time (0x8382AC4C: r6=double bits, f1=float), not the decompiler's HIDWORD(v30) garbage. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_variant.h"
#include "headers/game_engine_place.h"
#include "headers/get_score_type.h"
#include "headers/player_datum.h"
#include "headers/game_engine_message.h"
#include "headers/race_message.h"
#include "headers/multiplayer_game_text_string.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

extern void *datum_try_and_get(const data_array *data, int index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern game_variant *game_engine_get_variant(void);
extern game_engine_place game_engine_get_place(int player_index, get_score_type score_type);
extern uint16_t * get_place_name(game_engine_place place);

uint8_t race_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size)
{
    /* `message` spans the game_engine_message base ids (case 22 = show_score) plus the race_message
     * extension (31-37); string-list indices are multiplayer_game_text_string. */
    switch ( message )
    {
        case game_engine_message_show_score: /* 22 */
        {
            int16_t *subject = datum_try_and_get(player_data, message_data);
            if ( !subject || !datum_try_and_get(player_data, player_index) )
                return 0;

            const wchar_t *place_format;
            if ( game_engine_get_variant()->game_engine_variant.terminator.ignored == 2 )
            {
                if ( subject[99] == 1 )
                {
                    int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
                    place_format = (text_tag != -1) ? unicode_string_list_get_string(text_tag, _string_name_1_flag)
                            : empty_wide_string;
                    goto format_with_place;
                }
                int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
                const wchar_t *format = (text_tag == -1) ? empty_wide_string
                        : unicode_string_list_get_string(text_tag, _string_name_n_flags);
                int16_t lap_count = subject[99];
                game_engine_place place = game_engine_get_place(player_index, _get_score_team);
                uint16_t *place_name = get_place_name(place);
                usnprintf(buffer, buffer_size, format, place_name, lap_count);
                return 1;
            }
            else
            {
                if ( subject[99] + 1 > game_engine_get_variant()->universal_variant.score_to_win )
                {
                    int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
                    place_format = (text_tag != -1) ? unicode_string_list_get_string(text_tag, _string_name_all_laps_complete)
                            : empty_wide_string;
                    goto format_with_place;
                }
                int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
                const wchar_t *format = (text_tag == -1) ? empty_wide_string
                        : unicode_string_list_get_string(text_tag, _string_name_lap_n_of_total);
                int score_to_win = game_engine_get_variant()->universal_variant.score_to_win;
                int16_t lap_count = subject[99];
                game_engine_place place = game_engine_get_place(player_index, _get_score_team);
                uint16_t *place_name = get_place_name(place);
                usnprintf(buffer, buffer_size, format, place_name, lap_count + 1, score_to_win);
                return 1;
            }

format_with_place:
            {
                game_engine_place place = game_engine_get_place(player_index, _get_score_team);
                uint16_t *place_name = get_place_name(place);
                usnprintf(buffer, buffer_size, place_format, place_name);
                return 1;
            }
        }

        case race_message_you_completed_a_lap: /* 31 */
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            int16_t *player_again = datum_try_and_get(player_data, message_data);
            if ( !player_again || !player )
                return 0;
            /* stat[0] = race_statistics.last_lap_time (byte +196), ticks */
            float lap_time = (float)player->statistics.multiplayer_statistics.race_statistics.last_lap_time * SECONDS_PER_TICK;
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            const wchar_t *format = (text_tag == -1) ? empty_wide_string
                    : unicode_string_list_get_string(text_tag, _string_you_completed_lap_n_in_x_seconds);
            usnprintf(buffer, buffer_size, format, player_again[99], lap_time);
            return 1;
        }

        case race_message_ally_completed_a_lap: /* 32 */
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !player )
                return 0;
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            const wchar_t *format = (text_tag == -1) ? empty_wide_string
                    : unicode_string_list_get_string(text_tag, _string_ally_name_completed_a_lap_n);
            usnprintf(buffer, buffer_size, format, player->name,
                    player->statistics.multiplayer_statistics.ctf_statistics.flag_returns);
            return 1;
        }

        case race_message_enemy_completed_a_lap: /* 33 */
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !player )
                return 0;
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            const wchar_t *format = (text_tag == -1) ? empty_wide_string
                    : unicode_string_list_get_string(text_tag, _string_enemy_name_completed_a_lap);
            usnprintf(buffer, buffer_size, format, player->name,
                    player->statistics.multiplayer_statistics.ctf_statistics.flag_returns);
            return 1;
        }

        case race_message_you_touched_a_flag_rally: /* 34 */
        {
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            const wchar_t *string = (text_tag == -1) ? empty_wide_string
                    : unicode_string_list_get_string(text_tag, _string_you_scored_a_flag);
            ustrncpy(buffer, string, buffer_size);
            return 1;
        }

        case race_message_ally_touched_a_flag_rally: /* 35 */
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !player )
                return 0;
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            const wchar_t *format = (text_tag == -1) ? empty_wide_string
                    : unicode_string_list_get_string(text_tag, _string_ally_name_scored_a_flag);
            usnprintf(buffer, buffer_size, format, player->name);
            return 1;
        }

        case race_message_enemy_touched_a_flag_rally: /* 36 */
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !player )
                return 0;
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            const wchar_t *format = (text_tag == -1) ? empty_wide_string
                    : unicode_string_list_get_string(text_tag, _string_enemy_name_scored_a_flag);
            usnprintf(buffer, buffer_size, format, player->name);
            return 1;
        }

        case race_message_new_best_lap_time: /* 37 */
        {
            player_datum *player = datum_try_and_get(player_data, message_data);
            if ( !player )
                return 0;
            /* stat[2] = race_statistics.best_lap_time (byte +200), ticks */
            float best_lap_time = (float)player->statistics.multiplayer_statistics.race_statistics.best_lap_time * SECONDS_PER_TICK;
            int text_tag = tag_loaded(0x75737472u, "ui\\multiplayer_game_text");
            const wchar_t *format = (text_tag == -1) ? empty_wide_string
                    : unicode_string_list_get_string(text_tag, _string_new_best_lap_time_x);
            usnprintf(buffer, buffer_size, format, best_lap_time);
            return 1;
        }

        default:
            return 0;
    }
}
