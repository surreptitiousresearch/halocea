/* hud_messaging_update @0x836A4BB8 — per-frame HUD messaging renderer for one local player.
 *
 * Draws, in priority order, the objective message, the scripted help message, or the persistent
 * state message (whichever is active), then the timed pickup/objective message ring
 * (datum->messages[4]). Handles split-screen line budgeting, per-message fade, icon/button/text
 * element substitution inside a state message, and advances a text cursor down the screen.
 *
 * ============================ CAVEAT (reg-alloc-fail reconstruction) ============================
 * The decompiler flagged this function "local variable allocation has failed, the output may be
 * wrong!", with heavy OVERLAPPED 64-bit/128-bit register packing (v14/v16/v30/v72/v83/v84/v91/
 * v92/v93). Pointers, screen coordinates, packed pixel colors and the two rectangle2d cursor
 * structs were all stuffed into shared HIDWORD/LODWORD/WORDn slots. This reconstruction untangles
 * them into named variables by SEMANTIC intent; two areas could not be recovered bit-exactly and
 * are best-effort:
 *   1. hud_calculate_point's result pointer was decoded as garbage (~(v8^1)); it is reconstructed
 *      as &screen_point (the [sp+64] point2d the following code reads back as v86/v87).
 *   2. The exact halfword-to-rectangle2d.n[] mapping of the line_bounds/line_cursor pair (v84/v83)
 *      is inferred from render_state_text/rasterizer_draw_unicode_string usage (they read n[0]=y0,
 *      n[1]=x0 as the cursor origin); the y1/x1 edges are set to their plausible values. Field
 *      ORDER within the packed slots is not guaranteed identical to the original.
 *   3. The objective fade ratio was packed into a 128-bit slot (v30); recovered as
 *      uptime/fade_ticks clamped to 1.0.
 * Control flow, every call site, and all argument intent are preserved.
 * =============================================================================================== */

#include <wchar.h>
#include <stdint.h>
#include "headers/rectangle2d.h"
#include "headers/text_justification.h"
#include "headers/hud_flash_reverse_colors_flags.h"
#include "headers/hud_button_icon_range.h"
#include "headers/icon_flags.h"
#include "headers/point2d.h"
#include "headers/real_argb_color.h"
#include "headers/game_input_preferences.h"
#include "headers/hud_globals.h"
#include "headers/hud_messaging_globals.h"
#include "headers/hud_messaging_datum.h"
#include "headers/hud_scripted_globals.h"
#include "headers/hud_state_message_definition.h"
#include "headers/icon_hud_element_definition.h"
#include "headers/scenario.h"
#include "headers/render_globals.h"
#include "headers/item_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

/* --- engine globals --- */

/* --- callees (boundaries) --- */
#include "headers/hud_absolute_placement_definition.h"
#include "headers/hud_placement_definition.h"
#include "headers/bitmap_data.h"
#include "headers/hud_color_definition.h"
extern uint8_t cinematic_in_progress(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern uint8_t game_engine_hud_draw_messages(int player_index);
extern int hud_get_font_index(void);
extern int16_t local_player_count(void);
extern int game_time_get(void);
extern int16_t game_time_get_elapsed(void);
extern void hud_calculate_point(int16_t local_player_index, const hud_absolute_placement_definition *absolute_placement, const hud_placement_definition *placement, const bitmap_data *bitmap, uint8_t in_multiplayer, float override_scale, point2d *result);
extern void input_abstraction_get_local_player_preferences(int16_t local_player_index, game_input_preferences *preferences);
extern unsigned int get_flash_color(const hud_color_definition *hud_color_def, int reference_value);
extern real_argb_color *pixel32_to_real_argb_color(unsigned int pixel, real_argb_color *color);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void draw_string_set_indents(int16_t initial_indent, int16_t paragraph_indent);
extern void render_state_text(rectangle2d *bounds, rectangle2d *cursor_bounds, const wchar_t *text, uint8_t custom);
extern void render_state_bitmap(rectangle2d *bounds, unsigned int color, const icon_hud_element_definition *icon, unsigned int unresolved_dead_arg4);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern const wchar_t *hud_get_item_string(int index);
extern const wchar_t  *hcex_get_hud_message(int is_state_message, int message_index, int element_index);
extern void usprintf(wchar_t *string, const wchar_t *format, ...);
extern int compare_messages(const hud_message *lhs, const hud_message *rhs);
extern void            qsort(void *base, unsigned int num, unsigned int width,
                           int (*compare)(const void *, const void *));
extern double          pow(double base, double exponent);

/* __ROL4__(text_index, 1) is a 2x scale of a halfword index into a wchar_t (2-byte) text stream. */
#define ROL4_1(x) (((unsigned int)(x) << 1) | ((unsigned int)(x) >> 31))

void hud_messaging_update(int16_t local_player_index)
{
    if ( cinematic_in_progress()
      || local_player_index == -1
      || !game_engine_hud_draw_messages(local_player_get_player_index(local_player_index)) )
        return;

    int font_index = hud_get_font_index();

    /* split-screen == more than one local player and not in a HCEX coop merge */
    int is_split_screen = !(local_player_count() <= 1 || hcex_coop_local_player_index >= 0);

    point2d screen_point;   /* CAVEAT: reconstructed result pointer of hud_calculate_point */
    {
        int16_t player_count = local_player_count();
        hud_calculate_point(local_player_index, &hud_msg_def->absolute_placement,
            &hud_msg_def->placement, nullptr, (uint8_t)(player_count > 1), 0.0f, &screen_point);
    }

    short line_top = screen_point.__s1.y;

    /* font metrics live at halfword offsets 2/3/4 of the font tag definition data */
    uint16_t *font_data = TAG_GET(uint16_t, font_index);
    short font_leading = font_data[4];
    short line_height;
    if ( is_split_screen )
    {
        line_height = font_leading + font_data[2];
        line_top = screen_point.__s1.y - offset_0;
    }
    else
    {
        line_height = font_leading + font_data[3] + font_data[2];
    }
    short first_line_height = line_height;

    hud_messaging_datum *datum = &hud_messaging_globals->message_data[render.local_player_index];

    /* line budget: 4 lines normally, 3 in split-screen */
    int max_lines = 4 - (is_split_screen != 0);

    int objective_active = hud_messaging_globals->objective.message
                        && hud_messaging_globals->objective.uptime;
    int help_active = hud_scripted_globals->show_hud_help_text
                   && hud_messaging_globals->help_message != nullptr;
    int state_active = datum->state_message.valid
                    && (datum->state_message.state_message || datum->state_message.message_buffer[0]);

    rectangle2d line_bounds;   /* was &v84 */
    rectangle2d line_cursor;   /* was &v83 */
    real_argb_color state_color;
    real_argb_color text_color;
    unsigned int    pixel_color = 0;   /* packed ARGB used by render_state_bitmap */

    if ( objective_active || help_active || state_active )
    {
        game_input_preferences prefs;
        input_abstraction_get_local_player_preferences(local_player_index, &prefs);

        if ( objective_active )
        {
            hud_globals_definition *hg = hud_globals;
            int now = game_time_get();
            unsigned int flash = get_flash_color(&hg->messaging.objective_color,
                now + hud_messaging_globals->objective.uptime
                    - hg->messaging.objective_color.custom.objective.up_ticks
                    - hg->messaging.objective_color.custom.objective.fade_ticks);
            pixel32_to_real_argb_color(flash, &state_color);

            /* CAVEAT: fade ratio recovered from overlapped 128-bit pack (v30) */
            float fade_ticks = (float)hg->messaging.objective_color.custom.objective.fade_ticks;
            float objective_fade = (float)hud_messaging_globals->objective.uptime / fade_ticks;
            if ( objective_fade > 1.0f )
                objective_fade = 1.0f;
            state_color.n[0] = objective_fade * state_color.n[0];

            pixel_color =
                (((((unsigned char)(state_color.n[0] * 255.0f) << 8)
                 | (unsigned char)(state_color.n[1] * 255.0f)) << 8)
                 | (unsigned char)(state_color.n[2] * 255.0f)) << 8
                 | (unsigned char)(state_color.n[3] * 255.0f);
        }
        else if ( help_active )
        {
            if ( hud_messaging_globals->use_flash )
            {
                pixel_color = get_flash_color(&hud_globals->messaging.color,
                                  hud_messaging_globals->flash_start_time);
            }
            else if ( hud_globals->messaging.color.flash_flags & (1u << _hud_flash_reverse_colors_bit) )
            {
                pixel_color = hud_globals->messaging.color.flash_color;
            }
            else
            {
                pixel_color = hud_globals->messaging.color.color;
            }
            pixel32_to_real_argb_color(pixel_color, &state_color);
        }
        else /* state message */
        {
            state_color = hud_msg_def->state_color;
            pixel_color =
                (((((unsigned char)(state_color.n[0] * 255.0f) << 8)
                 | (unsigned char)(state_color.n[1] * 255.0f)) << 8)
                 | (unsigned char)(state_color.n[2] * 255.0f)) << 8
                 | (unsigned char)(state_color.n[3] * 255.0f);
        }

        /* set up the text column: origin at (screen_point.__s1.x, line_top), 5 lines tall */
        line_bounds.n[0] = line_top;                                                   /* y0 */
        line_bounds.n[1] = screen_point.__s1.x;                                             /* x0 */
        line_bounds.n[2] = 5 * line_height + line_top;                                 /* y1 */
        line_bounds.n[3] = render.camera.window_bounds.n[3] - render.camera.viewport_bounds.n[1]; /* x1 */
        line_cursor = line_bounds;

        draw_string_set_draw_mode(font_index, -1, _text_justification_left, 0, &state_color);

        int message_index = -1;
        hud_state_message_definition *message;

        if ( objective_active || help_active )
        {
            /* fall through to element-render section below */
        }
        else if ( !datum->state_message.state_message )
        {
            /* raw state text with no definition: just draw the assembled buffer */
            if ( datum->state_message.message_buffer[0] )
                render_state_text(&line_bounds, &line_cursor,
                    datum->state_message.message_buffer, 1u);
            goto finish_first_pass;
        }

        /* resolve the active message definition and its owning hud_messages tag data */
        cache_file_tag_instance *hud_messages_instance;
        if ( objective_active )
        {
            int16_t *p_uptime = &hud_messaging_globals->objective.uptime;
            int remaining = *p_uptime - game_time_get_elapsed();
            *p_uptime = remaining <= 0 ? 0 : (int16_t)remaining;
            message = hud_messaging_globals->objective.message;
            hud_messages_instance = TAG_INSTANCE_ELEMENT(global_scenario->hud_messages.index);
        }
        else if ( help_active )
        {
            message = hud_messaging_globals->help_message;
            hud_messages_instance = TAG_INSTANCE_ELEMENT(global_scenario->hud_messages.index);
        }
        else
        {
            message = datum->state_message.state_message;
            hud_messages_instance = TAG_INSTANCE_ELEMENT(hud_globals->messaging.hud_messages.index);
        }

        /* hud_messages tag data: [3]=text data ptr, [6]=element bytes ptr, [8]=count, [9]=definition array */
        /* BUGFIX vs prior source: disasm takes the ELEMENT address then reads +0x14; the old
         * TAG_INSTANCE macro already added +0x14, double-counting. */
        int *hud_messages_data = (int *)hud_messages_instance->base_address;
        int message_count = hud_messages_data[8];
        int text_cursor = ROL4_1(message->text_start_index) + hud_messages_data[3];

        if ( message_count > 0 )
        {
            int i = 0;
            /* recovered: (i << 6) + base -> &definitions[i]  (stride 64 == sizeof(hud_state_message_definition)) */
            hud_state_message_definition *definitions = (hud_state_message_definition *)hud_messages_data[9];
            while ( message != &definitions[i] )
            {
                i = (int16_t)(i + 1);
                if ( i >= message_count )
                    goto after_index_search;
            }
            message_index = i;
        }
after_index_search:
        if ( !message->element_count )
            goto finish_first_pass;

        int element = 0;
        while ( 1 )
        {
            unsigned char *element_bytes =
                (unsigned char *)(2 * (message->element_start_index + element) + hud_messages_data[6]);

            if ( !*element_bytes )
            {
                /* plain text element */
                const wchar_t *text = hcex_get_hud_message(state_active, message_index, element);
                if ( !text )
                    text = (const wchar_t *)text_cursor;
                render_state_text(&line_bounds, &line_cursor, text, 0);
                text_cursor += ROL4_1((unsigned char)element_bytes[1]);
                goto next_element;
            }

            unsigned int element_data = element_bytes[1];
            int16_t button_icon_index = -1;

            if ( element_data <= _hud_icon_specific_button_end )
            {
                button_icon_index = element_bytes[1];
            }
            else if ( element_data > _hud_icon_remapped_button_end )
            {
                /* icon / scenario-name slot */
                if ( !hud_scripted_globals->show_hud_help_text )
                {
                    int slot = (int16_t)(element_bytes[1] - _hud_icon_misc_start);
                    const icon_hud_element_definition *is_text =
                        (const icon_hud_element_definition *)
                            (datum->state_message.is_text_flags & (1 << (element_bytes[1] - _hud_icon_misc_start)));
                    if ( is_text )
                    {
                        int16_t string_index = datum->state_message.info[slot].text.string_index;
                        if ( (uint16_t)string_index == 0xFFFF )
                        {
                            render_state_text(&line_bounds, &line_cursor, L"<unknown>", 0);
                        }
                        else
                        {
                            const wchar_t *string;
                            if ( datum->state_message.info[slot].text.uses_scenario_names )
                                string = unicode_string_list_get_string(
                                    global_scenario->custom_object_names.index, string_index);
                            else
                                string = unicode_string_list_get_string(
                                    hud_globals->messaging.alternate_icon_text.index, string_index);
                            render_state_text(&line_bounds, &line_cursor, string, 0);
                        }
                    }
                    else
                    {
                        /* recovered: *(unsigned int *)&info[slot] -> info[slot].icon */
                        const icon_hud_element_definition *icon_bitmap = datum->state_message.info[slot].icon;
                        if ( icon_bitmap )
                            render_state_bitmap(&line_cursor, pixel_color, icon_bitmap, 0);
                    }
                    goto next_element;
                }
            }
            else if ( element_data > 0x1C )
            {
                button_icon_index = -1;
            }
            else
            {
                button_icon_index =
                    prefs.game_control_to_xbox_buttons[button_mappings[element_data - _hud_icon_remapped_button_start]];
            }

            if ( button_icon_index != -1 )
            {
                if ( button_icon_index >= hud_globals->messaging.button_icons.count )
                {
                    render_state_text(&line_bounds, &line_cursor, L"<no button icon>", 0);
                }
                else
                {
                    icon_hud_element_definition *icon =
                        &((icon_hud_element_definition *)hud_globals->messaging.button_icons.address)[button_icon_index];
                    if ( icon->flags & (1u << _hud_icon_use_text_bit) )   /* render as text instead of bitmap */
                    {
                        if ( icon->flags & (1u << _hud_icon_use_color_bit) )
                        {
                            pixel32_to_real_argb_color(pixel_color, &text_color);
                            draw_string_set_draw_mode(font_index, -1, _text_justification_left, 0, &text_color);
                        }
                        const wchar_t *icon_text = unicode_string_list_get_string(
                            hud_globals->messaging.alternate_icon_text.index, icon->text_index);
                        render_state_text(&line_bounds, &line_cursor, icon_text, 0);
                        draw_string_set_draw_mode(font_index, -1, _text_justification_left, 0, &state_color);
                    }
                    else
                    {
                        render_state_bitmap(&line_cursor, pixel_color, icon, 0);
                    }
                }
            }

next_element:
            element = (int16_t)(element + 1);
            if ( element >= message->element_count )
                break;
        }

finish_first_pass:
        draw_string_set_indents(0, 0);
        line_height = first_line_height;
        line_top = line_cursor.n[2];   /* CAVEAT: advanced-cursor y (was WORD2(v83)) */
    }

    /* ---- LABEL_93: timed pickup/objective message ring ---- */
    if ( !objective_active && !help_active
      && (datum->state_message.valid || datum->leave_first_line_blank) )
    {
        /* leave a blank line at the top; advance the cursor by one line of spacing */
        float advanced;
        if ( is_split_screen )
        {
            short base = screen_point.__s1.y - offset_0;
            advanced = hud_msg_def->spacing * (float)line_height + (float)base;
        }
        else
        {
            advanced = hud_msg_def->spacing * (float)line_height + (float)screen_point.__s1.y;
        }
        line_top = (short)advanced;
        --max_lines;
    }

    /* recovered: qsort(datum, 4u, 0x8Cu, ...) -> datum->messages (offset 0), sizeof(hud_message) */
    qsort(datum->messages, 4u, sizeof(hud_message), (int (*)(const void *, const void *))compare_messages);

    if ( max_lines > 0 )
    {
        rectangle2d msg_bounds;
        unsigned short formatted[276];
        int i = 0;
        do
        {
            hud_message *msg = &datum->messages[i];
            if ( !msg->valid )
                break;

            int now = game_time_get();
            int elapsed = now - msg->time;

            text_color = hud_msg_def->text_color;
            if ( (float)elapsed > (hud_msg_def->up_time * 30.0f) )
            {
                float fade = 1.0f
                    - ((float)elapsed - (hud_msg_def->up_time * 30.0f))
                        / (hud_msg_def->fade_time * 30.0f);
                if ( fade < 0.0f )
                    fade = 0.0f;
                else if ( fade > 1.0f )
                    fade = 1.0f;
                text_color.n[0] = text_color.n[0] * (float)pow(fade, 1.899999976158142);
            }

            /* build this message's bounds: origin (screen_point.__s1.x, line_top), one line tall */
            msg_bounds.n[0] = line_top;                                                        /* y0 */
            msg_bounds.n[1] = screen_point.__s1.x;                                                  /* x0 */
            msg_bounds.n[2] = line_height + line_top;                                          /* y1 */
            msg_bounds.n[3] = render.camera.window_bounds.n[3] - render.camera.viewport_bounds.n[1]; /* x1 */

            line_top = (short)(hud_msg_def->spacing * (float)line_height + (float)line_top);

            draw_string_set_draw_mode(font_index, -1, _text_justification_left, 0, &text_color);

            const wchar_t *item_text;
            if ( msg->item_definition_index == -1 )
            {
                item_text = msg->msg;
            }
            else
            {
                int message_offset = (signed char)msg->message_offset;
                if ( message_offset == -1 )
                    message_offset = (uint8_t)(local_player_count() <= 1);   /* single-player -> 1 */

                item_definition *item_data = TAG_GET(item_definition, msg->item_definition_index);
                item_text = hud_get_item_string(
                    (int16_t)(item_data->item.hud_message_index + (char)message_offset));

                if ( ((uint8_t)msg->message_offset == 255 && (char)message_offset)
                  || msg->quantity )
                {
                    int16_t divisor = item_data->item.hud_message_value_scale;
                    if ( divisor <= 1 )
                        divisor = 1;
                    usprintf(formatted, item_text, msg->quantity / divisor);
                    item_text = (const wchar_t *)formatted;
                }
            }

            rasterizer_draw_unicode_string(&msg_bounds, nullptr, nullptr, 0, item_text);

            int age = now - msg->time;
            unsigned char still_visible = (float)age
                < ((hud_msg_def->fade_time + hud_msg_def->up_time) * 30.0f);
            msg->valid = still_visible;
            if ( !still_visible )
                msg->time = -1;

            i = (int16_t)(i + 1);
        }
        while ( i < max_lines );
    }
}
