/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* game_engine_post_rasterize_post_game @0x83750CA0 — draws the multiplayer post-game (carnage report)
 * results screen: a full-screen background bitmap, an optional two-row team-score banner (team games
 * only, winning team's row drawn first), a column-header row, then one row per displayed player
 * (rank, name, score, kills, assists, deaths — each column color-coded by that player's postgame
 * rating), and finally a "waiting for host / press a button" caption faded by the post-game options
 * fade level. Player rows come from select_players_to_display (top 12 by score).
 *
 * DEVIATIONS from the decompiler output (verified against disasm_range 0x83750CC4..0x83751508):
 *  - The two packed rectangle/tab-stop constants the decompiler emitted as magic 64-bit literals
 *    ((rectangle2d)31457920LL and 0x12C015E019A01F4LL + LODWORD 3277000) are decoded field-by-field
 *    from the individual li/sth stores: the background rect is {y0=0,x0=0,y1=480,x1=640} and the
 *    team-score tab stops are {50,200,300,350,410,500}.
 *  - The header-row usnprintf has SIX "%s" args but the decompiler dropped the sixth (string 71,
 *    passed on the stack at var_70C); restored here.
 *  - The color/tab-stop stack slots the decompiler aliased across the winner-scan data_iterator and
 *    the later real_argb_colors are given distinct named locals (semantically identical — the callees
 *    copy the values out).
 *  - The final fade color is v45 = v39 with alpha replaced by post_game_options_fade; the decompiler
 *    rendered the n[2]/n[3] copy as a bogus window_bounds pointer deref (v34).
 *  - The background-bitmap tag def pointer read at TAG_INSTANCE(idx)+0x14 folds to base_address (the
 *    cache_file_tag_instance data pointer at 0x14), i.e. TAG_DEF(interface tag). */

#include <wchar.h>
#include <stdint.h>
#include "headers/statistic_buffer.h"
#include "headers/postgame_statistic.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/game_engine.h"
#include "headers/game_variant.h"
#include "headers/game_engine_globals.h"
#include "headers/hud_globals.h"
#include "headers/real_argb_color.h"
#include "headers/rectangle2d.h"
#include "headers/render_globals.h"
#include "headers/bitmap_data.h"
#include "headers/global_tag_instances.h"
#include "headers/point2d.h"
#include "headers/interface_tag_index.h"
#include "headers/draw_text_flags.h"
#include "headers/text_justification.h"
#include "headers/blam_data_globals.h"
#include "headers/multiplayer_game_text_string.h"

#include "headers/network_game_server.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/point2d.h"
extern const wchar_t empty_wide_string; /* .short 0 — empty wide string, "tag not loaded" fallback */

extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void draw_string_set_color(const real_argb_color *color);
extern void draw_string_set_format(int16_t style, int16_t justification, unsigned int flags);
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);
extern int interface_get_tag_index(int16_t interface_tag_index);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern void draw_bitmap_in_rect(bitmap_data *bitmap, rectangle2d *rect, rectangle2d *bitmap_rect, rectangle2d *clip_rect, unsigned int argb, rasterizer_dynamic_screen_geometry_parameters *multitexture_params, uint8_t no_plasma);
extern int game_engine_did_player_win_default(int player_index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);
extern void drawline(const wchar_t *text, int16_t line, int16_t justification);
extern int select_players_to_display(statistic_buffer *out_players, enum postgame_statistic statistic, int requested_count, int unused_max_capacity);
extern int postgame_statistic_get_rating(int player_index, enum postgame_statistic statistic, uint8_t inverse);
extern rectangle2d *offset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy);
extern void draw_string_and_hack_in_icons(rectangle2d *bounds, rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *instring, uint8_t ignore_icon_color);
extern network_game_server *global_network_game_server_get(void);

/* data pointer of the loaded tag whose index is `defidx`. */
#define TAG_DEF(defidx) (TAG_GET(char, defidx))

extern void get_postgame_hilite_colors(real_argb_color *normal, real_argb_color *column_hilite, real_argb_color *winner);
void game_engine_post_rasterize_post_game(void)
{
    if (!game_engine)
        return;

    const wchar_t *empty_string = &empty_wide_string;

    /* Column tab stops shared by the header and player rows. */
    int16_t tab_stops[6] = { 50, 125, 250, 350, 410, 500 };

    /* DEVIATION: inlined copy of get_postgame_hilite_colors@0x83746628 (zero-xref, fully inlined here) collapsed to a call; the three by-ref out-params map 1:1 to local_color/remote_color/good_rating_color, no folded scalar args. */
    /* Local-player highlight color (light blue) and the two "good rating" grays. */
    real_argb_color local_color, good_rating_color, remote_color;
    get_postgame_hilite_colors(&local_color, &remote_color, &good_rating_color);

    int font_index = hud_globals->messaging.single_player_font.index;
    draw_string_set_draw_mode(font_index, -1, _text_justification_center, (1u << _draw_text_bottom_justify_bit), &local_color);
    draw_string_set_color(&local_color);
    draw_string_set_format(-1, 0, 0);

    /* Full-screen background bitmap from the post-game interface tag's bitmap group. */
    hud_globals_definition *interface_def = (hud_globals_definition *)TAG_DEF(interface_get_tag_index(_interface_hud_globals));
    rectangle2d background_rect;
    background_rect.n[0] = 0;    /* y0 */
    background_rect.n[1] = 0;    /* x0 */
    background_rect.n[2] = 480;  /* y1 */
    background_rect.n[3] = 640;  /* x1 */
    /* _interface_hud_globals is the hud-globals tag; +980 = carnage_report_bitmap.index */
    if (bitmap_group_try_and_get_bitmap(interface_def->carnage_report_bitmap.index, 0))
    {
        bitmap_data *bitmap = bitmap_group_try_and_get_bitmap(interface_def->carnage_report_bitmap.index, 0);
        draw_bitmap_in_rect(bitmap, &background_rect, &background_rect, nullptr, 0xFFFFFFFF, nullptr, 1u);
    }

    /* The background rect's first dword is repurposed as a scratch pointer holding the empty-string
     * fallback (used as the "tag not loaded" instring for the rank column and the final caption). */
    *(const wchar_t **)background_rect.n = empty_string;

    if (global_variant.universal_variant.teams)
    {
        int16_t team_tab_stops[6] = { 50, 200, 300, 350, 410, 500 };

        /* format_index picks which of the two team-message strings each row uses; row order is
         * swapped so the winning team is drawn first. */
        int format_index[2] = { 0, 1 };

        /* Find the first non-quit player and ask the engine whether their team won. */
        int winner = 0;
        data_iterator winner_scan;
        data_iterator_new(&winner_scan, player_data);
        int *datum = data_iterator_next(&winner_scan);
        if (datum)
        {
            while (datum[8]) /* +0x20 != 0 → player quit; skip */
            {
                datum = data_iterator_next(&winner_scan);
                if (!datum)
                    goto after_winner;
            }
            if (game_engine)
            {
                int (__fastcall *did_player_win)(int) = game_engine->game_engine_did_player_win;
                if (did_player_win)
                    winner = did_player_win(winner_scan.index);
                else
                    winner = game_engine_did_player_win_default(winner_scan.index);
            }
        }
after_winner:;

        int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
        const wchar_t *team_message_format[2];
        team_message_format[0] = (text_tag == -1) ? empty_string
                : unicode_string_list_get_string(text_tag, _string_tab_red_team_tab_string);
        text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
        team_message_format[1] = (text_tag == -1) ? empty_string
                : unicode_string_list_get_string(text_tag, _string_tab_blue_team_tab_string);

        if (!winner)
        {
            format_index[0] = 1;
            format_index[1] = 0;
        }

        draw_string_set_tab_stops(team_tab_stops, 6);
        for (int row = 0; row < 2; row++)
        {
            int which = format_index[row];
            wchar_t team_score[512];
            game_engine->get_team_score_string(which, team_score);
            wchar_t line[256];
            usnprintf(line, 0x100u, team_message_format[which], team_score);
            drawline(line, row + 4, 0);
        }
    }

    /* Column headers: rank / name / score / kills / assists / deaths (strings 67..71 plus the
     * engine-provided score-column header). */
    int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *header_rank = (text_tag == -1) ? empty_string
            : unicode_string_list_get_string(text_tag, _string_place);
    text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *header_name = (text_tag == -1) ? empty_string
            : unicode_string_list_get_string(text_tag, _string_name);
    text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *header_col3 = (text_tag == -1) ? empty_string
            : unicode_string_list_get_string(text_tag, _string_kills);
    text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *header_col4 = (text_tag == -1) ? empty_string
            : unicode_string_list_get_string(text_tag, _string_assists);
    text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
    const wchar_t *header_col5 = (text_tag == -1) ? empty_string
            : unicode_string_list_get_string(text_tag, _string_deaths);

    wchar_t score_header[512];
    game_engine->get_score_header_string(score_header);

    wchar_t line[256];
    /* DEVIATION: sixth %s (header_col5) restored — decompiler dropped the stack-passed vararg. */
    usnprintf(line, 0x100u, L"\t%s\t%s\t%s\t%s\t%s\t%s",
            header_rank, header_name, score_header, header_col3, header_col4, header_col5);
    draw_string_set_tab_stops(tab_stops, 6);
    drawline(line, 7, 0);

    /* Up to 12 player rows, already sorted by score. */
    statistic_buffer selected_players[12];
    int player_count = select_players_to_display(selected_players, _postgame_statistic_ranking, 12, 0);
    if (player_count > 0)
    {
        int line_number = 8;
        statistic_buffer *entry = selected_players;
        int remaining = player_count;
        do
        {
            int player_index = entry->player_index;
            player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

            /* Local (split-screen) players get the highlight color, remote players white. */
            const real_argb_color *row_color = (const real_argb_color *)&remote_color;
            if (player->local_player_index == (int16_t)0xFFFF)
                row_color = &local_color;
            draw_string_set_color(row_color);
            draw_string_set_tab_stops(tab_stops, 6);

            /* Rank column: string (36 + rank), rank clamped to [0,31]. */
            int rank = entry->place & 0x7F;
            if (rank > 31)
                rank = 31;
            text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
            const wchar_t *rank_string = (text_tag == -1)
                    ? *(const wchar_t **)background_rect.n
                    : unicode_string_list_get_string(text_tag, rank + 36);
            usnprintf(line, 0x100u, L" \t%s", rank_string);
            drawline(line, line_number, 0);

            /* Name column, tinted by team in team games. */
            draw_string_set_color(&local_color);
            if (global_variant.universal_variant.teams)
            {
                real_argb_color team_colors[2];
                team_colors[0].n[0] = 1.0f;
                team_colors[0].n[1] = 0.80000001f;
                team_colors[0].n[2] = 0.40000001f;
                team_colors[0].n[3] = 0.40000001f;
                team_colors[1].n[0] = 1.0f;
                team_colors[1].n[1] = 0.40000001f;
                team_colors[1].n[2] = 0.40000001f;
                team_colors[1].n[3] = 0.80000001f;

                int team = player->team_index;
                if (team < 0)
                    team = 0;
                else if (team > 1)
                    team = 1;
                draw_string_set_color(&team_colors[team]);
            }
            usnprintf(line, 0x100u, L" \t \t%s", player->name);
            drawline(line, line_number, 0);

            /* Score column. */
            draw_string_set_color(&local_color);
            if (!postgame_statistic_get_rating(player_index, _postgame_statistic_custom, 0))
                draw_string_set_color(&good_rating_color);
            wchar_t score_string[512];
            game_engine->get_score_string(player_index, score_string);
            usnprintf(line, 0x100u, L" \t \t \t%s", score_string);
            drawline(line, line_number, 0);

            /* Kills column. */
            draw_string_set_color(&local_color);
            if (!postgame_statistic_get_rating(player_index, _postgame_statistic_kills, 0))
                draw_string_set_color(&good_rating_color);
            usnprintf(line, 0x100u, L" \t \t \t \t%d", player->statistics.kills[0]);
            drawline(line, line_number, 0);

            /* Assists column. */
            draw_string_set_color(&local_color);
            if (!postgame_statistic_get_rating(player_index, _postgame_statistic_assists, 0))
                draw_string_set_color(&good_rating_color);
            usnprintf(line, 0x100u, L" \t \t \t \t \t%d", player->statistics.assists[0]);
            drawline(line, line_number, 0);

            /* Deaths column. */
            draw_string_set_color(&local_color);
            if (!postgame_statistic_get_rating(player_index, _postgame_statistic_deaths, 0))
                draw_string_set_color(&good_rating_color);
            usnprintf(line, 0x100u, L" \t \t \t \t \t \t%d", player->statistics.deaths);
            drawline(line, line_number, 0);

            draw_string_set_tab_stops(tab_stops, 6);
            --remaining;
            ++line_number;
            entry++;
        } while (remaining);
    }

    /* Bottom caption, faded by the post-game options fade. */
    real_argb_color fade_color = local_color;
    fade_color.n[0] = game_engine_globals.post_game_options_fade;

    rectangle2d caption_rect;
    caption_rect.n[2] = render.camera.window_bounds.n[2]; /* y1 */
    caption_rect.n[3] = render.camera.window_bounds.n[3]; /* x1 */
    caption_rect.n[0] = 410; /* y0 */
    caption_rect.n[1] = 70;  /* x0 */
    offset_rectangle2d(&caption_rect, -render.camera.viewport_bounds.n[1],
            -render.camera.viewport_bounds.n[0]);
    draw_string_set_tab_stops(nullptr, 0);
    draw_string_set_color(&fade_color);

    const wchar_t *caption;
    if (global_network_game_server_get())
    {
        caption_rect.n[1] = 280;
        int caption_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
        caption = (caption_tag == -1) ? empty_string
                : unicode_string_list_get_string(caption_tag, _string_server_post_game_stats_footer);
    }
    else
    {
        caption_rect.n[1] = 420;
        int caption_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
        caption = (caption_tag == -1) ? empty_string
                : unicode_string_list_get_string(caption_tag, _string_client_post_game_stats_footer);
    }
    draw_string_and_hack_in_icons(&caption_rect, nullptr, nullptr, 0, caption, 0);
}
