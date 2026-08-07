/* draw_string_and_hack_in_icons @0x83731D88 — draw a string into `bounds`, splitting on '%' and
 * substituting each `%<icon-name>` run with the matching input-icon glyph (button prompts, stick
 * icons, etc.) via get_icon_type/remap_sticks_for_local_player/render_state_bitmap_0. Plain text runs
 * between icons go through rasterizer_draw_unicode_string. `clip`, `cursor_reference`, and
 * `height_adjust` are accepted (matching funcs.prototype) but never read in this function's body.
 *
 * DEVIATION: the DB's applied prototype for render_state_bitmap_0 names four params (bounds,
 * cursor_bounds, color, icon), but the callee is genuinely 3-param: only r3/r4/r5 are read before
 * being written in its prologue (0x83731878-0x83731880: r3->r29 bounds, r4->r28 fallback color,
 * r5->r31 icon), and every field access off r31 matches icon_hud_element_definition's real offsets
 * from types_members. r6 is NOT an argument slot at this call site: `or r6, r7, r10` @0x83732088 is a
 * scratch intermediate immediately consumed by `slwi r4, r6, 8` @0x8373208C while the color bytes are
 * packed into r4. Reconstructed from that disasm rather than the applied names; the callee's own TU
 * (render_state_bitmap_0.c) records the same 3-param conclusion.
 *
 * FAITHFUL QUIRK: `icon_color` is computed via pixel32_to_real_argb_color (and possibly overwritten
 * with a copy of `text_color`) but is never actually read afterward — the packed color that reaches
 * render_state_bitmap_0 is built entirely from `text_color`. Reproduced verbatim since the calls may
 * have side effects; only the dead *use* of the result is a quirk. */

#include <wchar.h>
#include <stdint.h>
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/real_argb_color.h"
#include "headers/game_input_preferences.h"
#include "headers/icon_hud_element_definition.h"
#include "headers/icon_flags.h"
#include "headers/hud_button_icon_range.h"
#include "headers/hud_globals.h"
#include "headers/blam_data_globals.h"


/* wcscpy provided by CRT via <wchar.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* wcschr provided by CRT via <wchar.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* wcslen provided by CRT via <wchar.h>; local non-standard extern removed (C2371 unsigned int vs size_t) */

extern void draw_string_set_indents(int16_t initial_indent, int16_t paragraph_indent);
extern void draw_unicode_string_compute_bounds(const rectangle2d *bounds, const wchar_t *string, rectangle2d *text_bounds, rectangle2d *cursor_bounds);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);
extern void render_state_text_0(rectangle2d *bounds, rectangle2d *cursor_bounds, const wchar_t *text);
extern int16_t get_icon_type(const wchar_t *string);
extern int16_t remap_sticks_for_local_player(int16_t icon, int16_t local_player_index);
extern void input_abstraction_get_local_player_preferences(int16_t local_player_index, game_input_preferences *preferences);
extern real_argb_color *pixel32_to_real_argb_color(unsigned int pixel, real_argb_color *color);
extern void draw_string_get_color(real_argb_color *color);

extern void render_state_bitmap_0(rectangle2d *bounds, unsigned int color, const icon_hud_element_definition *icon);

void draw_string_and_hack_in_icons(
        rectangle2d *bounds,
        rectangle2d *clip,
        point2d *cursor_reference,
        int16_t height_adjust,
        const wchar_t *instring,
        uint8_t ignore_icon_color)
{
    wcscpy(string_data, instring);

    rectangle2d local_bounds = *bounds;
    uint16_t *current = string_data;

    while (1)
    {
        uint16_t *percent_pos = wcschr(current, L'%');
        if (!percent_pos)
            break;
        *percent_pos = 0;
        uint16_t *icon_spec = percent_pos + 1;

        /* FAITHFUL: equivalent to the binary's carry-flag idiom for max(0, diff) */
        short indent_diff = (short)(local_bounds.__s1.x0 - bounds->__s1.x0);
        short initial_indent = (indent_diff < 0) ? 0 : indent_diff;
        draw_string_set_indents(initial_indent, 0);

        rectangle2d text_bounds;
        draw_unicode_string_compute_bounds(bounds, current, &text_bounds, &local_bounds);
        text_bounds.__s1.x0 = bounds->__s1.x0;
        local_bounds.__s1.x0 -= 3;

        rasterizer_draw_unicode_string(&text_bounds, 0, 0, 0, current);

        bounds->__s1.y0 = local_bounds.__s1.y0;
        current = icon_spec;

        int16_t icon_type = get_icon_type(icon_spec);
        if (icon_type == -1)
        {
            render_state_text_0(bounds, &local_bounds, L"%");
        }
        else
        {
            current = icon_spec + wcslen(icon_names[icon_type]);

            int16_t remapped = remap_sticks_for_local_player(icon_type,
                    local_player_index_for_draw_string_and_hack_in_icons);
            int16_t button_index = -1;
            if (remapped <= _hud_icon_specific_button_end)
            {
                button_index = remapped;
            }
            else if (remapped <= _hud_icon_remapped_button_end)
            {
                if (remapped > 28)
                {
                    switch (remapped)
                    {
                    case 28: button_index = 12; break;
                    case 29: button_index = 13; break;
                    case 30: button_index = 16; break;
                    default: button_index = 17; break;
                    }
                }
                else
                {
                    game_input_preferences preferences;
                    input_abstraction_get_local_player_preferences(
                            local_player_index_for_draw_string_and_hack_in_icons, &preferences);
                    button_index = preferences.game_control_to_xbox_buttons[
                            button_mappings_0[remapped - _hud_icon_remapped_button_start]];
                }
            }

            if (button_index != -1)
            {
                icon_hud_element_definition *icon =
                        &((icon_hud_element_definition *)hud_globals->messaging.button_icons.address)[button_index];

                uint8_t saved_flags = icon->flags;
                int16_t saved_width_offset = icon->width_offset;

                real_argb_color icon_color;
                pixel32_to_real_argb_color(icon->color, &icon_color); /* FAITHFUL QUIRK: result unused below */

                icon->flags &= ~(1u << _hud_icon_use_color_bit);
                if (icon_is_special[button_index])
                {
                    icon->width_offset = -5;
                    icon->flags &= ~(1u << _hud_icon_absolute_width_bit);
                }

                real_argb_color text_color;
                draw_string_get_color(&text_color);

                if (icon->color == 0 || ignore_icon_color)
                    icon_color = text_color; /* FAITHFUL QUIRK: still unused below — see file header note */

                unsigned int alpha_byte = (unsigned int)(text_color.__s1.alpha * 255.0f);

                text_color.__s1.rgb.__s1.green *= text_color.__s1.alpha;
                text_color.__s1.rgb.__s1.red *= text_color.__s1.alpha;
                text_color.__s1.rgb.__s1.blue *= text_color.__s1.alpha;

                unsigned int g_byte = (unsigned int)(text_color.__s1.rgb.__s1.green * 255.0f) & 0xFF;
                unsigned int r_byte = (unsigned int)(text_color.__s1.rgb.__s1.red * 255.0f) & 0xFF;
                unsigned int b_byte = (unsigned int)(text_color.__s1.rgb.__s1.blue * 255.0f) & 0xFF;

                /* FAITHFUL: byte order as packed in the binary is A,B,G,R (not the usual A,R,G,B) */
                unsigned int packed_color = (alpha_byte << 24) | (b_byte << 16) | (g_byte << 8) | r_byte;

                render_state_bitmap_0(&local_bounds, packed_color, icon);

                bounds->__s1.x0++;

                icon->flags = saved_flags;
                icon->width_offset = saved_width_offset;
            }
        }

        /* FAITHFUL: `current` is never actually null here (icon_spec/current always derive from a
         * non-null base), so this check never fires in practice; kept for fidelity to the binary. */
        if (!current)
            break;
    }

    if (current)
        render_state_text_0(bounds, &local_bounds, current);

    draw_string_set_indents(0, 0);
}
