/* ui_loading_update_and_render @0x837645F0 — per-frame update and draw for the network-connection loading
 * screen (background bitmap + a few lines of status text). Handles the closing fade-out, the action-button
 * abort/menu-transition per substate, and picks which status/hint strings to format for the current state.
 *
 * DEVIATION: an intermediate `HIDWORD(v2) = 0x82000000; v24 = v2;` in the decompiled output is dead — it is
 * unconditionally overwritten (via four `sth` half-word stores, not a real 64-bit assignment) before the
 * rect it aliases is ever read; omitted here as decompiler noise from stack-slot reuse.
 * DEVIATION: the decompiled `HIWORD(v24) += N; WORD2(v24) += N;` pairs are the rect's y0/y1 fields moving
 * a text line down by N pixels — confirmed field-by-field via disasm (`sth`s to var_500/var_500+4), so
 * written here as plain `rect.y0 += N; rect.y1 += N;` rather than reproducing the raw macro view. */

#include <wchar.h>
#include <stdint.h>
#include "headers/rectangle2d.h"
#include "headers/text_justification.h"
#include "headers/real_argb_color.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/bitmap_data.h"
#include <string.h>
#include "headers/game_connection.h"
#include "headers/loading_screen_string.h"
#include "headers/loading_screen_state.h"
#include "headers/control_button.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern uint32_t system_milliseconds(void);
extern uint8_t input_abstraction_is_action_button_down(int16_t local_player_index, int16_t button);
extern int main_connect(const char *ip_and_port, const char *password);
extern void main_goto_main_menu(void);
extern void network_game_abort(void);
extern int16_t game_connection(void);

extern int tag_loaded(uint32_t group_tag, const char *name);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern uint32_t modulate_pixel32_by_real_alpha(uint32_t argb, float alpha);
extern void draw_bitmap_in_rect(bitmap_data *bitmap, rectangle2d *rect, rectangle2d *bitmap_rect, rectangle2d *clip_rect, unsigned int argb, rasterizer_dynamic_screen_geometry_parameters *multitexture_params, uint8_t no_plasma);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void usprintf(wchar_t *string, const wchar_t *format, ...);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);

void ui_loading_update_and_render(void)
{
    if (!loading_screen_state)
        return;

    float alpha = 1.0f;

    if (loading_screen_open_time_ms == -1)
        loading_screen_open_time_ms = system_milliseconds();

    if (loading_screen_close_time_ms != -1)
    {
        unsigned int now = system_milliseconds();
        if (now >= loading_screen_close_time_ms)
        {
            loading_screen_close_time_ms = -1;
            loading_screen_state = 0;
            loading_screen_open_time_ms = -1;
            loading_screen_retry_count = 0;
            loading_screen_connect_ip[0] = 0;
            loading_screen_map[0] = 0;
            loading_screen_natneg_cookie = -1;
            return;
        }

        alpha = (loading_screen_close_time_ms - now) * 0.0013333333f; /* fade out over the last ~750ms */
        if (alpha >= 0.0f)
        {
            if (alpha > 1.0f)
                alpha = 1.0f;
        }
        else
        {
            alpha = 0.0f;
        }
    }

    if (input_abstraction_is_action_button_down(0, _button_back))
    {
        if (loading_screen_state >= LOADING_STATE_SERVER && loading_screen_state <= LOADING_STATE_WAITING_FOR_NEW_GAME)
        {
            switch (loading_screen_state)
            {
            case LOADING_STATE_RESOLVING:
                main_connect(0, 0);
                main_goto_main_menu();
                return;

            case LOADING_STATE_NEGOTIATING:
                if (loading_screen_natneg_cookie != -1)
                {
                    loading_screen_natneg_cookie = -1;
                    main_goto_main_menu();
                }
                break;

            case LOADING_STATE_LOADING_MP_MAP:
                break;

            case LOADING_STATE_SERVER:
            default:
                main_connect(0, 0);
                network_game_abort();
                return;
            }
        }
    }

    int font_index   = tag_loaded(0x666F6E74u /* 'font' */, "ui\\large_ui");
    int bitmap_group  = tag_loaded(0x6269746Du /* 'bitm' */, "ui\\shell\\bitmaps\\background");
    int strings_index = tag_loaded(0x75737472u /* 'ustr' */, "ui\\shell\\strings\\loading");

    if (font_index == -1 || bitmap_group == -1 || strings_index == -1)
        return;

    bitmap_data *background = bitmap_group_get_bitmap_from_sequence(bitmap_group, 0, 0);

    real_argb_color tint;
    /* DEVIATION: real_argb_color union wraps the alpha+rgb view in __s1 (DB layout). */
    tint.__s1.rgb.__s1.red = 1.0f;
    tint.__s1.rgb.__s1.green = 1.0f;
    tint.__s1.rgb.__s1.blue = 1.0f;
    tint.__s1.alpha = alpha;
    unsigned int background_argb = modulate_pixel32_by_real_alpha(0xFFFFFFFF, alpha);

    rectangle2d rect = { 0, 0, 480, 640 };

    if (background)
    {
        rasterizer_dynamic_screen_geometry_parameters multitexture_params;
        memset(&multitexture_params, 0, sizeof(multitexture_params));
        draw_bitmap_in_rect(background, &rect, &rect, 0, background_argb, &multitexture_params, 1);
    }

    draw_string_set_draw_mode(font_index, -1, _text_justification_center, 0, &tint);

    wchar_t line[512];
    /* DEVIATION: rectangle2d union wraps the named-edge view in __s1 (DB layout). */
    rect.__s1.y0 = 410;
    rect.__s1.x0 = 0;
    rect.__s1.y1 = 430;
    rect.__s1.x1 = 640;

    if (loading_screen_state >= LOADING_STATE_SERVER && loading_screen_state <= LOADING_STATE_WAITING_FOR_NEW_GAME)
    {
        switch (loading_screen_state)
        {
        case LOADING_STATE_SERVER:
            usprintf(line, unicode_string_list_get_string(strings_index, loading_string_server));
            break;

        case LOADING_STATE_RESOLVING:
        case LOADING_STATE_CONNECTING:
            usprintf(line, unicode_string_list_get_string(strings_index, loading_string_connecting), loading_screen_connect_ip);
            break;

        case LOADING_STATE_NEGOTIATING:
            usprintf(line, unicode_string_list_get_string(strings_index, loading_string_natneg), loading_screen_connect_ip);
            break;

        case LOADING_STATE_RETRYING:
            usprintf(line, unicode_string_list_get_string(strings_index, loading_string_retrying), loading_screen_connect_ip,
                     loading_screen_retry_count);
            break;

        case LOADING_STATE_CONNECTED:
            usprintf(line, unicode_string_list_get_string(strings_index, loading_string_connected));
            break;

        case LOADING_STATE_LOADING_MP_MAP:
        {
            int16_t string_index = (game_connection() == _game_connection_network_server) 
                    ? loading_string_server_loading : loading_string_connected_loading;
            usprintf(line, unicode_string_list_get_string(strings_index, string_index), loading_screen_map);
            break;
        }

        default: /* LOADING_STATE_WAITING_FOR_NEW_GAME */
            usprintf(line, unicode_string_list_get_string(strings_index, loading_string_waiting_for_new_game), loading_screen_map);
            break;
        }

        rasterizer_draw_unicode_string(&rect, 0, 0, 0, line);

        rect.__s1.y0 += 20;
        rect.__s1.y1 += 20;
        if (loading_screen_state != LOADING_STATE_LOADING_MP_MAP)
        {
            rasterizer_draw_unicode_string(&rect, 0, 0, 0,
                unicode_string_list_get_string(strings_index, loading_string_press_escape));
        }

        rect.__s1.y0 += 30;
        rect.__s1.y1 += 30;
        rasterizer_draw_unicode_string(&rect, 0, 0, 0,
            unicode_string_list_get_string(strings_index, loading_string_esrb_warning));
    }
}
