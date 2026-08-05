/* game_engine_generate_title_string @0x83750800 — builds the scoreboard title/header string for one
 * player. First computes a "lives remaining" status text (used later only by the free-for-all branch):
 * empty once lives run out, "1 life left"-style at exactly 1, an snprintf'd count otherwise. Then:
 *   - during the postgame delay, shows a win/lose/tie(-1) message picked via the game engine's own
 *     game_engine_did_player_win callback (falling back to game_engine_did_player_win_default), varying
 *     by team vs free-for-all; an unrecognized win-result value leaves the caller's buffer untouched
 *     (just null-terminates) — a genuine, faithful quirk, not corrected.
 *   - otherwise, free-for-all shows the player's rank + score + lives status; team games show a win/lose/
 *     tie score comparison. The tie case is asymmetric on purpose (only team 1's score string is
 *     formatted in, not both) — confirmed via the decompile's own argument list, not a transcription slip.
 *
 * Clean decompile (no corruption warning), just large. `player->statistics.deaths` at player_datum+174 and
 * the player-indexing idiom are both already-established conventions from this session's
 * populate_statistic_buffer/adjust_score_for_ranking work. */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/game_engine_globals.h"
#include "headers/game_variant.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/statistic_buffer.h"
#include "headers/multiplayer_game_text_string.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string; /* .short 0 — empty wide string, "tag not loaded" fallback */

extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern int game_engine_did_player_win_default(int player_index);
extern statistic_buffer * game_engine_get_player_place(statistic_buffer *result, int player_index);
extern uint16_t * get_place_string(statistic_buffer *buffer);

void game_engine_generate_title_string(int player_index, uint16_t *title_string)
{
    uint16_t lives_count_buffer[128];
    const wchar_t *lives_status_text = &empty_wide_string;

    if (global_variant.universal_variant.lives > 0)
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
        int lives_used = player->statistics.deaths;
        int lives_remaining = global_variant.universal_variant.lives - lives_used;

        if (global_variant.universal_variant.lives == lives_used)
        {
            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            lives_status_text = (text_tag != -1) ? unicode_string_list_get_string(text_tag, _string_no_lives) : &empty_wide_string;
        }
        else if (lives_remaining == 1)
        {
            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            lives_status_text = (text_tag != -1) ? unicode_string_list_get_string(text_tag, _string_1_life) : &empty_wide_string;
        }
        else
        {
            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            const wchar_t *format = (text_tag != -1) ? unicode_string_list_get_string(text_tag, _string_n_lives)
                    : &empty_wide_string;
            usnprintf(lives_count_buffer, 0x80u, format, lives_remaining);
            lives_count_buffer[127] = 0;
            lives_status_text = lives_count_buffer;
        }
    }

    if (game_engine_globals.mode == game_engine_mode_postgame_delay)
    {
        int did_win = 0;
        if (game_engine)
        {
            did_win = game_engine->game_engine_did_player_win ? game_engine->game_engine_did_player_win(player_index)
                    : game_engine_did_player_win_default(player_index);
        }
        uint8_t teams = game_engine ? global_variant.universal_variant.teams : 0;

        switch (did_win)
        {
        case -1:
        {
            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            if (text_tag != -1)
            {
                ustrncpy(title_string, unicode_string_list_get_string(text_tag, _string_game_ends_in_a_draw), 0x50u);
                title_string[79] = 0;
                return;
            }
            break;
        }
        case 0:
        {
            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            if (text_tag != -1)
            {
                ustrncpy(title_string, unicode_string_list_get_string(text_tag, teams ? 56 : 57), 0x50u);
                title_string[79] = 0;
                return;
            }
            break;
        }
        case 1:
        {
            int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            if (text_tag != -1)
            {
                ustrncpy(title_string, unicode_string_list_get_string(text_tag, teams ? 58 : 59), 0x50u);
                title_string[79] = 0;
                return;
            }
            break;
        }
        default:
            title_string[79] = 0;
            return;
        }

        ustrncpy(title_string, &empty_wide_string, 0x50u);
        title_string[79] = 0;
        return;
    }

    uint8_t teams = game_engine ? global_variant.universal_variant.teams : 0;
    if (!teams)
    {
        statistic_buffer place_stats;
        game_engine_get_player_place(&place_stats, player_index);

        uint16_t score_string[296];
        game_engine->get_score_string(player_index, score_string);

        int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
        const wchar_t *format = (text_tag != -1)
                ? unicode_string_list_get_string(text_tag, (place_stats.place >= 0) ? 64 : 63)
                : &empty_wide_string;

        uint16_t *place_string = get_place_string(&place_stats);
        usnprintf(title_string, 0x50u, format, place_string, score_string, lives_status_text);
        title_string[79] = 0;
        return;
    }

    uint16_t team0_score_string[14];
    uint16_t team1_score_string[14];
    game_engine->get_team_score_string(0, team0_score_string);
    game_engine->get_team_score_string(1, team1_score_string);
    int team0_score = game_engine->get_team_score(0);
    int team1_score = game_engine->get_team_score(1);

    if (team0_score > team1_score)
    {
        int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
        const wchar_t *format = (text_tag != -1) ? unicode_string_list_get_string(text_tag, _string_red_leads_blue_score_to_score_lives) : &empty_wide_string;
        usnprintf(title_string, 0x50u, format, team0_score_string, team1_score_string, lives_status_text);
    }
    else if (team0_score < team1_score)
    {
        int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
        const wchar_t *format = (text_tag != -1) ? unicode_string_list_get_string(text_tag, _string_blue_leads_red_score_to_score_lives) : &empty_wide_string;
        usnprintf(title_string, 0x50u, format, team1_score_string, team0_score_string, lives_status_text);
    }
    else
    {
        int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
        const wchar_t *format = (text_tag != -1) ? unicode_string_list_get_string(text_tag, _string_teams_tied_lives) : &empty_wide_string;
        /* asymmetric on purpose — only team 1's score string is formatted in for a tie, see file header */
        usnprintf(title_string, 0x50u, format, team1_score_string, lives_status_text);
    }
    title_string[79] = 0;
}
