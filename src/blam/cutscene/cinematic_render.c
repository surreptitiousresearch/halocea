/* cinematic_render @0x83691280 — per-frame cinematic overlay draw: animates and draws the letterbox
 * bars, then draws up to 4 active cinematic subtitle ("chapter title") strings with fade in/out.
 *
 * DEVIATION: the DB's decompiler output is a wall of __int64/LODWORD/HIDWORD/SWORD-punned register
 * reuse (rectangle2d locals round-tripped through 64-bit stack slots for int/float conversion, and a
 * scaled-bounds computation that looked like it read `viewport_bounds.n[0]` as a lone 16-bit field in
 * several places when it's actually a 32-bit paired read of two adjacent rectangle2d fields at once).
 * Rebuilt the letterbox-bar geometry and the tag-bounds scaling block from disasm_range(0x836912D0,
 * 0x83691448) and disasm_range(0x836914F0, 0x8369160C) rather than trusting those decompiler casts.
 *
 * Confirmed via types_members: hud_globals->messaging.single_player_font is a tag_reference at offset
 * 0x48 (was previously undifferentiated padding — added to hud_globals.h); the 96-byte
 * scenario.cutscene_chapter_titles element matches scenario_cutscene_title's DB layout exactly at
 * every field this function touches (bounds@0x28, text_index@0x30, style@0x32, justification@0x34,
 * text_flags@0x38, foreground_color@0x3C, shadow_color@0x40, fade_in_time@0x44, up_time@0x48,
 * fade_out_time@0x4C); and hud_globals->defaults.default_title_bounds@0x2DC (0x2C0+0x1C) matches the
 * disasm's `ld r8, 0x2DC(r9)` fallback load exactly. */

#include <stdint.h>
#include <stddef.h>
#include "headers/cinematic_globals.h"
#include "headers/render_globals.h"
#include "headers/hud_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_cutscene_title.h"
#include "headers/unicode_string_list_group_header.h"
#include "headers/global_tag_instances.h"
#include "headers/real_argb_color.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/game_time_constants.h"

#include "headers/point2d.h"
extern double __fabs(double x);
extern uint8_t ui_widgets_active(void);
extern int game_time_get(void);
extern uint8_t game_time_get_paused(void);
extern int16_t game_time_get_elapsed(void);
extern uint8_t game_in_editor(void);
extern void draw_quad(rectangle2d *rect, unsigned int color);
extern real_argb_color *pixel32_to_real_argb_color(unsigned int pixel, real_argb_color *color);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void rasterizer_text_set_shadow_color(unsigned int color);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);

void cinematic_render(void)
{
    if ((cinematic_globals->letter_box || cinematic_globals->letter_box_amount > 0.0f) && !ui_widgets_active())
    {
        int now = game_time_get();
        int last = cinematic_globals->letter_box_last_game_time;
        cinematic_globals->letter_box_last_game_time = now;
        int delta_ticks = now - last;

        float amount;
        if (cinematic_globals->letter_box)
        {
            amount = (float)delta_ticks * SECONDS_PER_TICK + cinematic_globals->letter_box_amount;
            cinematic_globals->letter_box_amount = amount; /* FAITHFUL: stored unclamped here, then again below */
            if (amount > 1.0f)
                amount = 1.0f;
        }
        else
        {
            amount = cinematic_globals->letter_box_amount - (float)delta_ticks * SECONDS_PER_TICK;
            cinematic_globals->letter_box_amount = amount; /* FAITHFUL: stored unclamped here, then again below */
            if (amount <= 0.0f)
                amount = 0.0f;
        }
        cinematic_globals->letter_box_amount = amount;

        if (cinematic_globals->letter_box_amount > 0.0f)
        {
            int viewport_height = render.camera.viewport_bounds.y1 - render.camera.viewport_bounds.y0;
            float bar_height = (float)viewport_height * (cinematic_globals->letter_box_amount * 0.125f);

            rectangle2d bar;
            bar.y0 = render.camera.viewport_bounds.y0;
            bar.x0 = render.camera.viewport_bounds.x0;
            bar.x1 = render.camera.viewport_bounds.x1;
            bar.y1 = (short)((float)render.camera.viewport_bounds.y0 + bar_height);
            draw_quad(&bar, 0xFF000000);

            bar.y1 = render.camera.viewport_bounds.y1;
            bar.x0 = render.camera.viewport_bounds.x0;
            bar.x1 = render.camera.viewport_bounds.x1;
            bar.y0 = (short)((float)render.camera.viewport_bounds.y1 - bar_height);
            draw_quad(&bar, 0xFF000000);
        }
    }

    for (int slot = 0; slot < 4; slot++)
    {
        cinematic_title_datum *active_title = &cinematic_globals->active_titles[slot];
        int16_t title_index = active_title->title_index;
        if (title_index == -1)
            continue;

        int font_index = hud_globals->messaging.single_player_font.index;
        if (font_index == -1)
            continue;

        int help_text_tag_index = global_scenario->ingame_help_text.index;
        scenario_cutscene_title *title = (scenario_cutscene_title *)global_scenario->cutscene_chapter_titles.address
                + title_index;
        if (help_text_tag_index == -1)
            continue;

        unicode_string_list_group_header *string_list =
                TAG_GET(unicode_string_list_group_header, help_text_tag_index);
        if (title->text_index < 0 || title->text_index >= string_list->string_references.count)
            continue;

        /* Scale the tag-authored (640x480-relative) title bounds to the actual viewport; if the
         * tag's bounds are degenerate (zero width or height — the "unset" sentinel), fall back to
         * hud_globals->defaults.default_title_bounds. */
        rectangle2d bounds = title->bounds;
        if (bounds.x1 == bounds.x0 || bounds.y1 == bounds.y0)
            bounds = hud_globals->defaults.default_title_bounds;

        int viewport_width = render.camera.viewport_bounds.x1 - render.camera.viewport_bounds.x0;
        int viewport_height = render.camera.viewport_bounds.y1 - render.camera.viewport_bounds.y0;

        rectangle2d scaled_bounds;
        scaled_bounds.x0 = (short)(bounds.x0 * viewport_width / 640);
        scaled_bounds.x1 = (short)(bounds.x1 * viewport_width / 640);
        scaled_bounds.y0 = (short)(bounds.y0 * viewport_height / 480);
        scaled_bounds.y1 = (short)(bounds.y1 * viewport_height / 480);

        float fade_fraction = 1.0f;
        if (!game_in_editor())
        {
            float title_timer = (float)active_title->title_timer;
            if (title_timer < title->fade_in_time)
            {
                fade_fraction = title_timer / title->fade_in_time;
            }
            else if (title_timer > title->up_time)
            {
                fade_fraction = 1.0f - (title_timer - title->up_time) / title->fade_out_time;
            }

            if (fade_fraction >= 0.0f)
            {
                if (fade_fraction > 1.0f)
                    fade_fraction = 1.0f;
            }
            else
            {
                fade_fraction = 0.0f;
            }
        }

        real_argb_color color;
        pixel32_to_real_argb_color(title->foreground_color, &color);
        color.alpha = color.alpha * fade_fraction;
        /* FAITHFUL QUIRK: pure-white foreground colors get their RGB clamped to 0.8 (avoids a fully
         * blown-out white subtitle) — a special case that only fires when the tag color is exactly
         * (1,1,1) within epsilon. */
        if (__fabs(color.rgb.n[0] - 1.0f) < 0.000099999997f
                && __fabs(color.rgb.n[1] - 1.0f) < 0.000099999997f
                && __fabs(color.rgb.n[2] - 1.0f) < 0.000099999997f)
        {
            if (color.rgb.n[0] > 0.80000001f) color.rgb.n[0] = 0.80000001f;
            if (color.rgb.n[1] > 0.80000001f) color.rgb.n[1] = 0.80000001f;
            if (color.rgb.n[2] > 0.80000001f) color.rgb.n[2] = 0.80000001f;
        }

        draw_string_set_draw_mode(font_index, title->style - 1, title->justification, title->text_flags, &color);

        unsigned int shadow_color = title->shadow_color;
        int scaled_shadow_alpha = (int)((float)(unsigned char)(shadow_color >> 24) * fade_fraction);
        if (scaled_shadow_alpha >= 0)
        {
            if (scaled_shadow_alpha > 255)
                scaled_shadow_alpha = 255;
        }
        else
        {
            scaled_shadow_alpha = 0;
        }
        rasterizer_text_set_shadow_color(((unsigned int)scaled_shadow_alpha << 24) | (shadow_color & 0xFFFFFF));

        const wchar_t *string = unicode_string_list_get_string(help_text_tag_index, title->text_index);
        rasterizer_draw_unicode_string(&scaled_bounds, 0, 0, 0, string);
        rasterizer_text_set_shadow_color(0);

        int16_t elapsed = game_time_get_paused() ? 0 : game_time_get_elapsed();
        active_title->title_timer += elapsed;

        if (!game_in_editor())
        {
            float total_duration = title->fade_out_time + title->up_time;
            if ((float)active_title->title_timer >= total_duration)
            {
                active_title->title_index = -1;
                active_title->title_timer = -1;
            }
        }
    }
}
