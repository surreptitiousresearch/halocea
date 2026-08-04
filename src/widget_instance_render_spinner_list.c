/* widget_instance_render_spinner_list @0x83733B18 — render a spinner-list widget: its extended-description
 * child (if any), the header/footer arrow bitmaps (each with a normal/pressed/hot-highlighted frame chosen
 * from the widget's last tab direction plus whether the mouse is hovering that arrow while hot), and — when
 * the definition has no child widgets — a text label naming the currently selected item (from a dynamic
 * unicode-string-list lookup with search-and-replace substitution, or the cached list item text), drawn with
 * a focus/definition color and an optional sine pulse.
 *
 * Unlike widget_instance_render_text_box's persistent `text_box_parameters.text` cache, this function builds
 * its dynamic string fresh every render call and disposes it again at the end (no out-of-memory fallback
 * string either — an allocation failure just skips the draw for this frame).
 *
 * DEVIATIONS ("local variable allocation has failed"; every reg-alloc-fail spot below is disasm-resolved,
 * 0x83733B18-0x8373412C):
 *  - The alpha_modifier parent-chain "loop" the decompiler renders as `for (i = 0; parent; ...)` folds in an
 *    unrelated `i = 0` initializer (a completely separate frame-index local, live at the same point purely by
 *    register coincidence) — the real loop is just `while (parent) { alpha *= parent->alpha_modifier; parent
 *    = parent->parent; }`, reproduced as a plain `for` over `parent` alone.
 *  - `widget->parameters.list_parameters.current_list_item_index` and `widget->parameters.text_box_parameters.
 *    string_list_index` are the SAME field (both at parameters+4) — this is a genuine list-type widget, so
 *    the list_parameters name is used.
 *  - The header/footer bitmap-rect and text bounds' `HIWORD(offset)`/`(_WORD)offset` pairs are a plain
 *    per-axis translation (`rect.x0/x1 += offset.x; rect.y0/y1 += offset.y`), confirmed by the disasm doing
 *    exactly four adds with no other data flow; the decompiler's stray `0x82000000 +`/`(0x82000000 >> 16) +`
 *    terms in the text-bounds version are pure noise (no corresponding instruction exists — same class of
 *    artifact documented in widget_instance_render_text_box.c).
 *  - The `rasterizer_dynamic_screen_geometry_parameters` zero-fill loop is a plain zero-init (and, per
 *    draw_bitmap_in_rect's own DEVIATION note, is dead in this build regardless — that function ignores both
 *    the params and no_plasma arguments).
 *  - The color-resolution `if (var_18C==1.0 && var_188==1.0 && var_184==1.0) f11=... else f11=...` branch
 *    computes the SAME value (`definition->text_color.n[0]`) on both arms once traced — collapsed to a
 *    single unconditional read below (kept the sibling text_box.c convention of an initial, later-overwritten
 *    `color.n[0] = global_real_argb_white->n[0]` in the focus branch for structural parity with that file).
 *  - `widget->parameters.list_parameters.spin_activated` and `.text_box_parameters.flashing` are the SAME
 *    byte (parameters+0x18) reinterpreted; this function unconditionally zeroes spin_activated earlier in
 *    the same call, so the `flashing` term of the pulse-gate OR is always false in practice — the pulse only
 *    ever fires via `definition->text_box_flags & (1<<_text_box_flashing_text_bit)`. Reproduced faithfully (not "fixed").
 *  - The pulse formula here is `((float)sin(ms*0.003) + 1.0) * 0.5` — a different curve from
 *    widget_instance_render_text_box's `(cos(ms*0.003)+1.5)*0.4`; each function's own disasm was checked
 *    independently rather than assumed to match its sibling. */

#include <wchar.h>
#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/search_and_replace_reference.h"
#include "headers/ui_widget_definition.h"
#include "headers/widget_event_flags.h"
#include "headers/bitmap_group.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include "headers/text_justification.h"


#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern uint8_t widget_instance_is_hot(widget_instance *widget);
extern int mouse_over_spinner_arrow(widget_instance *widget);
extern void widget_instance_render_recursive(widget_instance *widget, rectangle2d *clip_rect, point2d offset, uint8_t focus, uint8_t use_nifty_plasma_fx);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern void draw_bitmap_in_rect(bitmap_data *bitmap, rectangle2d *rect, rectangle2d *bitmap_rect, rectangle2d *clip_rect, unsigned int argb, rasterizer_dynamic_screen_geometry_parameters *multitexture_params, uint8_t no_plasma);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern unsigned int ustrlen(const wchar_t *string);
extern void *pool_new_pointer(stack_memory_pool *pool, unsigned int size);
extern void dispose_pointer(stack_memory_pool *pool, void *p);
/* memcpy provided by CRT via <wchar.h>/<stdint.h> chain; local non-standard extern removed (C4028 size_t vs unsigned int) */
extern uint16_t *ui_widget_search_and_replace_invoke(widget_instance *widget, int16_t function);
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern int search_and_replace(const wchar_t *search, const wchar_t *replace, uint16_t **in_buffer);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);
extern double sin(double x);

void widget_instance_render_spinner_list(widget_instance *widget, ui_widget_definition *definition,
        rectangle2d *clip_rect, point2d offset, uint8_t focus)
{
    float alpha = widget->alpha_modifier;
    for (widget_instance *parent = widget->parent; parent; parent = parent->parent)
        alpha = (parent->alpha_modifier * alpha);

    unsigned __int8 is_hot = widget_instance_is_hot(widget);
    int mouse_over_arrow = mouse_over_spinner_arrow(widget);

    if (!widget->visible)
        return;

    if (widget->parameters.list_parameters.extended_description)
    {
        widget_instance *extended_description = widget->parameters.list_parameters.extended_description;
        float description_alpha = widget->alpha_modifier;
        for (widget_instance *parent = widget->parent; parent; parent = parent->parent)
            description_alpha = (parent->alpha_modifier * description_alpha);
        extended_description->alpha_modifier = description_alpha;
        widget_instance_render_recursive(extended_description, clip_rect, offset, 0, 1);
    }

    int header_frame_state = 0;
    int footer_frame_state = 0;
    __int16 last_list_tab_direction = widget->parameters.list_parameters.last_list_tab_direction;
    if (last_list_tab_direction)
    {
        if (last_list_tab_direction < 0)
        {
            widget->parameters.list_parameters.last_list_tab_direction = last_list_tab_direction + 1;
            header_frame_state = 1;
        }
        else
        {
            widget->parameters.list_parameters.last_list_tab_direction = last_list_tab_direction - 1;
            footer_frame_state = 1;
        }
    }
    widget->parameters.list_parameters.spin_activated = 0;

    /* Header (previous/left) arrow bitmap. */
    int header_bitmap_index = definition->list_header_bitmap.index;
    bitmap_group *header_group = TAG_GET(bitmap_group, header_bitmap_index);
    if (is_hot && header_group && header_group->bitmaps.count == 4 && widget_instance_is_hot(widget) && mouse_over_arrow <= 0)
        header_frame_state += 2;

    bitmap_data *header_bitmap = bitmap_group_get_bitmap_from_sequence(header_bitmap_index, 0, header_frame_state);
    if (header_bitmap)
    {
        rasterizer_dynamic_screen_geometry_parameters multitexture_params = {0};
        rectangle2d header_bounds = definition->list_header_bounds;
        header_bounds.x0 += offset.x;
        header_bounds.x1 += offset.x;
        header_bounds.y0 += offset.y;
        header_bounds.y1 += offset.y;
        unsigned int argb = ((unsigned int)(int)(alpha * 255.0f) << 24) | 0xFFFFFF;
        draw_bitmap_in_rect(header_bitmap, &header_bounds, &header_bounds, clip_rect, argb, &multitexture_params, 0);
    }

    /* Footer (next/right) arrow bitmap. */
    int footer_bitmap_index = definition->list_footer_bitmap.index;
    bitmap_group *footer_group = TAG_GET(bitmap_group, footer_bitmap_index);
    if (is_hot && footer_group && footer_group->bitmaps.count == 4 && widget_instance_is_hot(widget) && mouse_over_arrow > 0)
        footer_frame_state += 2;

    bitmap_data *footer_bitmap = bitmap_group_get_bitmap_from_sequence(footer_bitmap_index, 0, footer_frame_state);
    if (footer_bitmap)
    {
        rasterizer_dynamic_screen_geometry_parameters multitexture_params = {0};
        rectangle2d footer_bounds = definition->list_footer_bounds;
        footer_bounds.x0 += offset.x;
        footer_bounds.x1 += offset.x;
        footer_bounds.y0 += offset.y;
        footer_bounds.y1 += offset.y;
        unsigned int argb = ((unsigned int)(int)(alpha * 255.0f) << 24) | 0xFFFFFF;
        draw_bitmap_in_rect(footer_bitmap, &footer_bounds, &footer_bounds, clip_rect, argb, &multitexture_params, 0);
    }

    if (definition->child_widget_references.count != 0)
        return;

    /* Resolve the item text: either a fresh unicode-string-list lookup (with search-and-replace), or the
     * cached selected-item text the list logic already produced. */
    unsigned __int16 *item_text;
    unsigned __int8 built_dynamic_text = definition->text_label_unicode_string_list_tag.index != -1;

    if (built_dynamic_text)
    {
        __int16 string_index = widget->parameters.list_parameters.current_list_item_index;
        const unsigned __int16 *string = unicode_string_list_get_string(
                definition->text_label_unicode_string_list_tag.index, string_index);
        unsigned int byte_length = 2 * ustrlen(string);
        unsigned __int16 *new_text = pool_new_pointer(widget_memory_pool, byte_length + 2);
        item_text = new_text;

        if (new_text)
        {
            memcpy(new_text, string, byte_length);
            new_text[byte_length / 2] = 0;

            for (int i = 0; i < definition->search_and_replace_references.count; ++i)
            {
                search_and_replace_reference *entry =
                    &((search_and_replace_reference *)definition->search_and_replace_references.address)[i];
                if (entry && entry->search_string[0])
                {
                    unsigned __int16 *replacement = ui_widget_search_and_replace_invoke(widget, entry->replace_function);
                    unsigned __int16 wide_buffer[32];
                    unsigned __int16 *find_string = ascii_to_wide(entry->search_string, wide_buffer, 0x40);
                    search_and_replace(find_string, replacement, &item_text);
                }
            }
        }
    }
    else
    {
        item_text = widget->parameters.list_parameters.item_text;
    }

    if (item_text && definition->font_tag.index != -1 && (unsigned int)definition->justification <= _text_justification_center)
    {
        float k = widget->alpha_modifier;
        for (widget_instance *parent = widget->parent; parent; parent = parent->parent)
            k = (parent->alpha_modifier * k);

        rectangle2d clip = clip_rect ? *clip_rect : definition->bounds;
        rectangle2d bounds = definition->bounds;
        bounds.x0 += offset.x;
        bounds.x1 += offset.x;
        bounds.y0 += offset.y;
        bounds.y1 += offset.y;

        real_argb_color color;
        float alpha_multiplier;
        if (focus)
        {
            color.n[0] = global_real_argb_white->n[0]; /* overwritten below; kept for parity with the
                                                          * widget_instance_render_text_box sibling */
            color.n[1] = global_ui_white_red;
            color.n[2] = global_ui_white_green;
            color.n[3] = global_ui_white_blue;
            alpha_multiplier = definition->text_color.n[0];
        }
        else
        {
            color = definition->text_color;
            alpha_multiplier = definition->text_color.n[0];
        }
        color.n[0] = k * alpha_multiplier;

        /* widget->parameters.list_parameters.spin_activated aliases .text_box_parameters.flashing (same
         * byte) and was just zeroed above, so this OR's left term is always false; the pulse only fires via
         * the definition flag. */
        unsigned char flashing = widget->parameters.text_box_parameters.flashing;
        if (flashing || (definition->text_box_flags & (1u << _text_box_flashing_text_bit)))
        {
            unsigned int ms = widget_globals.current_system_milliseconds;
            double pulse = sin((float)ms * 0.003f);
            color.n[0] = ((float)pulse + 1.0f) * 0.5f * color.n[0];
        }

        draw_string_set_draw_mode(definition->font_tag.index, -1, definition->justification, 0, &color);
        rasterizer_draw_unicode_string(&bounds, &clip, 0, 0, item_text);
    }

    if (built_dynamic_text)
        dispose_pointer(widget_memory_pool, item_text);
}
