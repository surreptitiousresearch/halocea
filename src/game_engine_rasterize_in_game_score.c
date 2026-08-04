/* game_engine_rasterize_in_game_score @0x83752740 — draws the in-game (non-postgame) scoreboard
 * overlay: a title row, a column-header row, then up to 16 player rows (grouped by team if the
 * variant has teams, one team per pass so each team's players are contiguous), followed by the
 * current game-variant/mode name caption at a fixed screen position.
 *
 * DEVIATION: the DB's decompiler mis-maps several call args and stack locals here:
 *  - select_players_to_display's own applied prototype names its params (player_index,
 *    out_statistic_buffer, postgame_statistic, maximum_count), but this call site's disasm
 *    (disasm_range(0x83752764, 0x8375279C)) shows the local player-row buffer landing in the FIRST
 *    GPR slot and 0/16 in the 2nd/3rd — see select_players_to_display.c's own header comment for the
 *    corrected roles, confirmed by decompiling that function directly.
 *  - The scratch buffer the decompiler split into `v51[24]`/`v52` is really one
 *    `statistic_buffer selected_players[16]` array (28-byte elements) that the huge stack frame
 *    confused the decompiler's local-variable allocator into fragmenting; rebuilt as a proper array
 *    here rather than reproducing the pointer arithmetic literally.
 *  - The hardcoded `__int64 0x1B8000A01CC027B` rectangle constant for the mode-name caption is a
 *    packed rectangle2d; decoded via disasm_range(0x83752C0C, 0x83752C40) to {y0=440, x0=10, y1=460,
 *    x1=635} (confirmed field-by-field from the individual `li`/`sth` instructions, not guessed from
 *    the hex).
 *
 * Confirmed via types_members: player_datum.statistics (offset 0x090) is a game_statistics struct
 * whose `deaths` field sits at relative offset 0x1E (absolute 0xAE) — matches the raw `*((__int16*)
 * player+87)` read exactly; this is the "lives used" counter compared against the variant's lives
 * limit. player_datum.team_index@0x020 and .quit_out_of_game@0x0D5 and .name@0x004 and
 * .unit_index@0x034 all matched their existing header offsets exactly against this function's raw
 * dword/byte-index reads. */

#include <wchar.h>
#include <stdint.h>
#include "headers/statistic_buffer.h"
#include "headers/text_justification.h"
#include "headers/postgame_statistic.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/real_argb_color.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/multiplayer_team.h"
#include "headers/blam_data_globals.h"
#include "headers/multiplayer_game_text_string.h"

#include "headers/point2d.h"
extern const wchar_t empty_wide_string; /* .short 0 — empty wide string, used as a "tag not loaded" fallback */

extern int  select_players_to_display(statistic_buffer *out_players, enum postgame_statistic statistic,
        int requested_count, int unused_max_capacity);
extern void game_engine_generate_title_string(int player_index, uint16_t *title_string);
extern void rasterize_in_game_score_draw_line(const wchar_t *text, uint8_t hilite, real_argb_color *_color, int line, int tab_stop_index);
extern real_argb_color * hud_get_text_color(real_argb_color *result);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void usprintf(wchar_t *string, const wchar_t *format, ...);
extern void *datum_try_and_get(const data_array *data, int index);
extern int16_t get_variant_name_id(void);
extern int hud_get_font_index(void);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);

void game_engine_rasterize_in_game_score(int player_index, float alpha)
{
    unsigned char teams = 0;
    if (game_engine)
        teams = global_variant.universal_variant.teams;

    wchar_t title_string[80];
    game_engine_generate_title_string(player_index, title_string);

    statistic_buffer selected_players[16];
    int player_count = select_players_to_display(selected_players, _postgame_statistic_ranking, 16, 0);

    real_argb_color title_color;
    title_color.n[0] = alpha;
    title_color.n[1] = 0.69999999f;
    title_color.n[2] = 0.69999999f;
    title_color.n[3] = 0.69999999f;
    unsigned char is_viewer = 0; /* title/header rows never belong to the viewer */
    rasterize_in_game_score_draw_line(title_string, is_viewer, &title_color, 0, 0);

    int viewer_team = -1; /* -1 = no team filter yet; becomes the viewer's own team once known */

    real_argb_color text_color_raw;
    hud_get_text_color(&text_color_raw);

    /* Team-game row colors: index 0 = reddish (team 0), index 1 = bluish (team 1). */
    real_argb_color team_colors[2];
    team_colors[0].n[0] = alpha;
    team_colors[0].n[1] = 0.60000002f;
    team_colors[0].n[2] = 0.30000001f;
    team_colors[0].n[3] = 0.30000001f;
    team_colors[1].n[0] = alpha;
    team_colors[1].n[1] = 0.30000001f;
    team_colors[1].n[2] = 0.30000001f;
    team_colors[1].n[3] = 0.60000002f;

    /* Non-team row color: the HUD's own text color. */
    real_argb_color individual_color;
    individual_color.n[0] = alpha;
    individual_color.n[1] = text_color_raw.n[1];
    individual_color.n[2] = text_color_raw.n[2];
    individual_color.n[3] = text_color_raw.n[3];

    /* Header row color: medium gray. */
    real_argb_color header_color;
    header_color.n[0] = alpha;
    header_color.n[1] = 0.5f;
    header_color.n[2] = 0.5f;
    header_color.n[3] = 0.5f;

    int text_tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *rank_header = text_tag_index == -1 ? &empty_wide_string
            : unicode_string_list_get_string(text_tag_index, _string_place);
    text_tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *name_header = text_tag_index == -1 ? &empty_wide_string
            : unicode_string_list_get_string(text_tag_index, _string_name);

    wchar_t score_header[512];
    game_engine->get_score_header_string(score_header);

    wchar_t line_buffer[256];
    usprintf(line_buffer, L"\t%s\t%s\t%s", rank_header, name_header, score_header);
    rasterize_in_game_score_draw_line(line_buffer, is_viewer, &header_color, 1, 0);

    void *viewer = datum_try_and_get(player_data, player_index);
    if (viewer)
        viewer_team = ((player_datum *)viewer)->team_index;

    int row = 0;
    int passes = (teams != 0) + 1; /* 2 passes (one per team) if the variant has teams, else 1 */
    while (passes > 0)
    {
        for (int i = 0; i < player_count; i++)
        {
            statistic_buffer *entry = &selected_players[i];
            is_viewer = entry->player_index == player_index;
            if (!datum_try_and_get(player_data, entry->player_index))
                continue;

            player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, entry->player_index);

            if (teams)
            {
                if (viewer_team != player->team_index)
                    continue;
            }

            wchar_t score_string[512];
            game_engine->get_score_string(entry->player_index, score_string);

            unsigned char out_of_lives = 0;
            if (global_variant.universal_variant.lives > 0)
            {
                if (player->unit_index == -1)
                    out_of_lives = player->statistics.deaths >= global_variant.universal_variant.lives;
            }

            const wchar_t *status_text;
            if (out_of_lives)
            {
                text_tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
                status_text = text_tag_index == -1 ? &empty_wide_string
                        : unicode_string_list_get_string(text_tag_index, _string_dead);
            }
            else if (!player->quit_out_of_game)
            {
                status_text = score_string;
            }
            else
            {
                text_tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
                status_text = text_tag_index == -1 ? &empty_wide_string
                        : unicode_string_list_get_string(text_tag_index, _string_quit);
            }

            int rank = 31;
            if ((entry->place & 0x7F) <= 0x1F)
                rank = entry->place & 0x7F;
            text_tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            const wchar_t *rank_text = text_tag_index == -1 ? &empty_wide_string
                    : unicode_string_list_get_string(text_tag_index, rank + 36);

            usprintf(line_buffer, L"\t%s\t%s\t%s", rank_text, player->name, status_text);

            real_argb_color *row_color;
            if (teams)
            {
                int team_index = player->team_index;
                if (team_index >= _multiplayer_team_red)
                {
                    if (team_index > _multiplayer_team_blue)
                        team_index = _multiplayer_team_blue;
                    row_color = &team_colors[team_index];
                }
                else
                {
                    row_color = &team_colors[0];
                }
            }
            else
            {
                row_color = &individual_color;
            }

            rasterize_in_game_score_draw_line(line_buffer, is_viewer, row_color, row + 2, 0);
            row++;
        }

        if (viewer_team != -1)
            viewer_team = 1 - viewer_team;
        passes--;
    }

    __int16 variant_name_id = get_variant_name_id();
    if (variant_name_id == 8)
        return;

    text_tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *variant_description;
    if (text_tag_index == -1)
    {
        variant_description = &empty_wide_string;
    }
    else
    {
        variant_description = unicode_string_list_get_string(text_tag_index, variant_name_id);
        if (!variant_description)
            return; /* tag loaded but this variant's description string is missing: draw nothing */
    }

    unsigned char team_game = 0;
    if (game_engine)
        team_game = global_variant.universal_variant.teams;

    /* FAITHFUL: the original calls tag_loaded again here from each of the team/non-team branches
     * separately (identical group tag + name both times) rather than sharing the first lookup;
     * reproduced as a single call since the two call sites are behaviorally identical. */
    int game_mode_tag_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    int mode_string_index = team_game == 1 ? 12 : 13;

    const wchar_t *game_mode_name;
    if (game_mode_tag_index == -1)
    {
        game_mode_name = &empty_wide_string;
        usprintf(line_buffer, L"%s (%s)", variant_description, game_mode_name);
    }
    else
    {
        game_mode_name = unicode_string_list_get_string(game_mode_tag_index, mode_string_index);
        if (!game_mode_name)
            usprintf(line_buffer, L"%s", variant_description);
        else
            usprintf(line_buffer, L"%s (%s)", variant_description, game_mode_name);
    }

    real_argb_color mode_name_color = *global_real_argb_white;
    mode_name_color.n[0] = alpha;

    /* {y0=440, x0=10, y1=460, x1=635} — decoded from the packed __int64 constant via disasm */
    const rectangle2d mode_name_bounds = {{440, 10, 460, 635}};

    int font_index = hud_get_font_index();
    draw_string_set_draw_mode(font_index, -1, _text_justification_right, 0, &mode_name_color);
    rasterizer_draw_unicode_string(&mode_name_bounds, 0, 0, 0, line_buffer);
}
