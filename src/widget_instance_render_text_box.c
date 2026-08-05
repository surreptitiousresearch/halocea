/* widget_instance_render_text_box @0x83733678 — draw a text-box widget's string: refresh the string
 * from a unicode string list tag if configured, run search-and-replace token substitution, resolve
 * the draw color (widget-level override, else focus color, else definition color with a pure-white
 * shortcut), apply the focus-alpha chain and an optional cos()-pulse flash, then draw via the
 * icon-aware or plain unicode string renderer depending on whether a '%'-escaped icon is present.
 *
 * DEVIATION: the decompiler's own inferred prototype shows a bogus 6th `long double a6` parameter and
 * types the 4th parameter as `point2d *offset`. The DB's real prototype (confirmed via disasm — r6 is
 * stored and re-read as two raw halfwords, never dereferenced as a pointer) is 5 args ending in
 * `point2d offset` BY VALUE; `a6` is decompiler noise from stack-scratch reuse in the cos() call, not a
 * real argument.
 *
 * DEVIATION: the decompiler represents the color-resolution logic as a `__PAIR64__`/`LODWORD`/`HIDWORD`-
 * laden mess reading `widget->parameters.list_parameters.*` fields. Traced via disasm: every one of
 * those accesses lands squarely inside `widget->parameters.text_box_parameters.text_color` (both
 * `_text_box_parameters` and `_list_parameters` share the same union storage; the decompiler simply
 * picked the wrong union arm to display for some of the loads). The real operation in every branch is a
 * plain 16-byte `real_argb_color` struct copy; rewritten as such below. Same for the two `*(_DWORD*)v24.n
 * = 0x82000000`-style lines seen mid-function elsewhere in this codebase's decompiles — this function's
 * own instance of that pattern (a stray `lis` half-address load) was dropped as pure decompiler noise
 * (no corresponding store exists in the disassembly). */

#include <wchar.h>
#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/search_and_replace_reference.h"
#include "headers/ui_widget_definition.h"
#include "headers/widget_event_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/text_justification.h"


/* memcpy provided by CRT via <wchar.h>/<stdint.h> chain; local non-standard extern removed (C4028 size_t vs unsigned int) */
extern double cos(double x);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern unsigned int ustrlen(const wchar_t *string);
extern void *pool_resize_pointer(stack_memory_pool *pool, void *p, unsigned int new_size);
extern uint16_t *ui_widget_search_and_replace_invoke(widget_instance *widget, int16_t function);
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern int search_and_replace(const wchar_t *search, const wchar_t *replace, uint16_t **in_buffer);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);
extern uint16_t *wcschr(const wchar_t *string, wchar_t ch);
extern int16_t get_icon_type(const wchar_t *string);
extern void draw_string_and_hack_in_icons(rectangle2d *bounds, rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *instring, uint8_t ignore_icon_color);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);

void widget_instance_render_text_box(widget_instance *widget, ui_widget_definition *definition,
                                     rectangle2d *clip_rect, point2d offset, uint8_t focus)
{
    if (definition->text_label_unicode_string_list_tag.index != -1)
    {
        int16_t string_list_index = widget->parameters.text_box_parameters.string_list_index;
        if (string_list_index == -1)
            string_list_index = definition->string_list_index;

        const uint16_t *string = unicode_string_list_get_string(
            definition->text_label_unicode_string_list_tag.index, string_list_index);
        unsigned int byte_length = 2 * ustrlen(string);
        uint16_t *new_text = pool_resize_pointer(
            widget_memory_pool, widget->parameters.text_box_parameters.text, byte_length + 2);
        widget->parameters.text_box_parameters.text = new_text;

        if (new_text)
        {
            memcpy(new_text, string, byte_length);
            widget->parameters.text_box_parameters.text[byte_length / 2] = 0;
        }
        else
        {
            widget->parameters.text_box_parameters.text = L"<out of memory>";
        }
    }

    uint16_t **text_field = &widget->parameters.text_box_parameters.text;
    uint16_t *text = *text_field;
    if (!text || !*text)
        return;

    if (definition->search_and_replace_references.count > 0)
    {
        search_and_replace_reference *entry =
            (search_and_replace_reference *)definition->search_and_replace_references.address;
        for (int i = 0; i < definition->search_and_replace_references.count; ++i, ++entry)
        {
            if (entry && entry->search_string[0])
            {
                uint16_t *replacement = ui_widget_search_and_replace_invoke(widget, entry->replace_function);
                uint16_t wide_buffer[32];
                uint16_t *find_string = ascii_to_wide(entry->search_string, wide_buffer, 0x40);
                search_and_replace(find_string, replacement, text_field);
            }
        }
    }

    if (definition->font_tag.index == -1 || (unsigned int)definition->justification > _text_justification_center || !widget->visible)
        return;

    float alpha = widget->alpha_modifier;
    for (widget_instance *parent = widget->parent; parent; parent = parent->parent)
        alpha = (parent->alpha_modifier * alpha);

    rectangle2d bounds = definition->bounds;
    rectangle2d clip = clip_rect ? *clip_rect : definition->bounds;

    int16_t hoffset = definition->hoffset;
    int16_t voffset = definition->voffset;
    bounds.x1 += offset.x;
    bounds.y1 += offset.y;
    bounds.x0 += hoffset + offset.x;
    bounds.y0 += voffset + offset.y;

    real_argb_color color;
    float alpha_multiplier;

    if (widget->parameters.text_box_parameters.text_color.n[0] != 0.0f)
    {
        color = widget->parameters.text_box_parameters.text_color;
        alpha_multiplier = color.n[0];
    }
    else if (focus)
    {
        color.n[0] = global_real_argb_white->n[0];
        color.n[1] = global_ui_white_red;
        color.n[2] = global_ui_white_green;
        color.n[3] = global_ui_white_blue;
        alpha_multiplier = definition->text_color.n[0];
    }
    else
    {
        color = definition->text_color;
        if (color.n[1] != 1.0f || color.n[2] != 1.0f || color.n[3] != 1.0f)
        {
            alpha_multiplier = color.n[0];
        }
        else
        {
            alpha_multiplier = definition->text_color.n[0];
            color.n[0] = global_real_argb_white->n[0];
            color.n[1] = global_ui_white_red;
            color.n[2] = global_ui_white_green;
            color.n[3] = global_ui_white_blue;
        }
    }

    unsigned char flashing = widget->parameters.text_box_parameters.flashing;
    color.n[0] = alpha * alpha_multiplier;

    if (flashing || (definition->text_box_flags & (1u << _text_box_flashing_text_bit)))
    {
        int current_ms = widget_globals.current_system_milliseconds;
        double pulse = cos((float)current_ms * 0.003f);
        color.n[0] = ((float)pulse + 1.5f) * 0.4f * color.n[0];
    }

    draw_string_set_draw_mode(definition->font_tag.index, -1, definition->justification, 0, &color);

    unsigned char has_icon = 0;
    uint16_t *scan = *text_field;
    while (scan)
    {
        uint16_t *percent = wcschr(scan, '%');
        if (!percent)
            break;
        if (get_icon_type(percent + 1) != -1)
        {
            has_icon = 1;
            break;
        }
        scan = percent + 1;
    }

    uint16_t *final_text = *text_field;
    if (has_icon)
        draw_string_and_hack_in_icons(&bounds, &clip, 0, 0, final_text, 0);
    else
        rasterizer_draw_unicode_string(&bounds, &clip, 0, 0, final_text);
}
