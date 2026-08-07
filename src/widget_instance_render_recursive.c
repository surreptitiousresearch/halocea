/* widget_instance_render_recursive @0x83734E48 — draws one widget_instance subtree: accumulates alpha
 * through the parent chain, computes this widget's screen offset (parent offset + this widget's
 * horizontal/vertical_offset), fires any game-data-driven input-value callbacks, draws the background
 * bitmap (with an optional "nifty plasma" tint pulse while drawing and a time-based alpha pulse for
 * widgets with _widget_flash_background_bitmap_bit), dispatches to the type-specific renderer (text box /
 * spinner list / column list), recurses into children (unless a list widget suppressed it), then dispatches
 * any _widget_event_post_render (33) event handlers.
 *
 * DEVIATION: several decompiler lines here are dead/mistracked display artifacts, confirmed absent from the
 * disasm and omitted: a bogus `*(_DWORD*)bounds.n = 0x82000000` write right after `bounds = i->bounds` (the
 * real instruction is a single 64-bit `std` copy of the whole rectangle2d, no such write exists), and two
 * `HIDWORD(v21) = (unsigned __int16)v39.n[0]` / `v38.n[3]` placeholder assignments before the offset-adjust
 * additions (no corresponding store exists in the disasm; both are decompiler tracking noise for the shared
 * 64-bit temporary holding the rectangle copy). */

#include <stdint.h>
#include <string.h>
#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/game_data_input_reference.h"
#include "headers/event_record.h"
#include "headers/event_handler_reference.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/widget_event_flags.h"
#include "headers/blam_data_globals.h"


extern double cos(double x);
extern void ui_widget_game_data_function_invoke(widget_instance *widget, uint16_t function);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern void draw_bitmap_in_rect(bitmap_data *bitmap, rectangle2d *rect, rectangle2d *bitmap_rect, rectangle2d *clip_rect, unsigned int argb, rasterizer_dynamic_screen_geometry_parameters *multitexture_params, uint8_t no_plasma);
extern void widget_instance_render_text_box(widget_instance *widget, ui_widget_definition *definition, rectangle2d *clip_rect, point2d offset, uint8_t focus);
extern void widget_instance_render_spinner_list(widget_instance *widget, ui_widget_definition *definition, rectangle2d *clip_rect, point2d offset, uint8_t focus);
extern void widget_instance_render_column_list(widget_instance *widget, ui_widget_definition *definition, rectangle2d *clip_rect, point2d offset, uint8_t focus);
extern void event_handler_dispatch(widget_instance *widget, ui_widget_definition *definition, event_record *event, event_handler_reference *event_handler, unsigned char *calling_widget_deleted);

void widget_instance_render_recursive(widget_instance *widget, rectangle2d *clip_rect, point2d offset,
                                      uint8_t focus, uint8_t use_nifty_plasma_fx)
{
    float alpha_modifier = widget->alpha_modifier;
    for (widget_instance *ancestor = widget->parent; ancestor; ancestor = ancestor->parent)
        alpha_modifier = (ancestor->alpha_modifier * alpha_modifier);

    ui_widget_definition *definition = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);

    uint8_t nifty_plasma_fx = use_nifty_plasma_fx;
    if (!use_nifty_plasma_fx && (definition->flags & (1u << _widget_always_render_with_nifty_fx_bit)) != 0)
        nifty_plasma_fx = 1;

    int16_t widget_offset_x = widget->horizontal_offset + offset.n[0];
    int16_t widget_offset_y = widget->vertical_offset   + offset.n[1];
    point2d child_offset;
    child_offset.n[0] = widget_offset_x;
    child_offset.n[1] = widget_offset_y;

    for (int i = 0; i < definition->game_data_input_references.count; i++)
    {
        int16_t function = ((game_data_input_reference *)definition->game_data_input_references.address)[i].function;
        ui_widget_game_data_function_invoke(widget, function);
    }

    if (!widget->visible)
        return;

    bitmap_data *background_bitmap = bitmap_group_get_bitmap_from_sequence(
        definition->background_bitmap.index, 0, widget->animation_data.current_frame_index);
    if (background_bitmap)
    {
        rectangle2d draw_rect = definition->bounds;
        draw_rect.n[1] += widget_offset_x;
        draw_rect.n[3] += widget_offset_x;
        draw_rect.n[0] += widget_offset_y;
        draw_rect.n[2] += widget_offset_y;

        rectangle2d adjusted_clip_rect;
        rectangle2d *effective_clip_rect = clip_rect;
        if (clip_rect)
        {
            adjusted_clip_rect = *clip_rect;
            adjusted_clip_rect.n[0] += widget_offset_y;
            adjusted_clip_rect.n[1] += widget_offset_x;
            adjusted_clip_rect.n[3] += widget_offset_x;
            adjusted_clip_rect.n[2] += widget_offset_y;
            effective_clip_rect = &adjusted_clip_rect;
        }

        if (nifty_plasma_fx)
        {
            ui_plasma_effect_color.n[0] = 0.0f;
            ui_plasma_effect_color.n[1] = 0.05f;
            ui_plasma_effect_color.n[2] = 0.05f;
            ui_plasma_effect_color.n[3] = 0.05f;
        }

        if ((definition->flags & (1u << _widget_flash_background_bitmap_bit)) != 0)
        {
            /* pulse alpha at ~0.003 rad/ms (~2.7s period), remapped from [-1,1] to [0,1] */
            float phase = (float)widget_globals.current_system_milliseconds * 0.003f;
            alpha_modifier = (((float)cos(phase) + 1.0f) * 0.5f) * alpha_modifier;
        }

        rasterizer_dynamic_screen_geometry_parameters multitexture_params;
        memset(&multitexture_params, 0, sizeof(multitexture_params));

        unsigned int argb = ((unsigned int)(int)(alpha_modifier * 255.0f) << 24) | 0xFFFFFF;
        draw_bitmap_in_rect(background_bitmap, &draw_rect, &draw_rect, effective_clip_rect, argb,
                            &multitexture_params, 0);

        if (nifty_plasma_fx)
        {
            ui_plasma_effect_color.n[0] = 0.0f;
            ui_plasma_effect_color.n[1] = 0.0f;
            ui_plasma_effect_color.n[2] = 0.0f;
            ui_plasma_effect_color.n[3] = 0.0f;
        }
    }

    uint8_t should_recurse_children = 1;
    switch ((uint16_t)widget->widget_type)
    {
    case _ui_widget_type_text_box:
    {
        uint8_t text_box_focus;
        if ((definition->text_box_flags & (1u << _text_box_no_weird_focus_test)) != 0)
        {
            text_box_focus = focus;
        }
        else
        {
            /* focus is inherited up the parent chain: true if this widget (or the first ancestor
             * whose parent doesn't treat it as the focused child) sits under a spinner/column list
             * ancestor that currently has it focused. */
            widget_instance *ancestor = widget->parent;
            uint8_t inherited_focus;
            if (ancestor)
                inherited_focus = (widget == ancestor->focused_child);
            else
                inherited_focus = 1;

            if (!inherited_focus && ancestor)
            {
                widget_instance *parent_of_ancestor;
                do
                {
                    parent_of_ancestor = ancestor->parent;
                    if (parent_of_ancestor)
                    {
                        if (parent_of_ancestor->focused_child != ancestor)
                        {
                            inherited_focus = 0;
                            break;
                        }
                        int parent_widget_type = parent_of_ancestor->widget_type;
                        inherited_focus |= (parent_widget_type == _ui_widget_type_spinner_list
                                            || parent_widget_type == _ui_widget_type_column_list);
                    }
                    ancestor = parent_of_ancestor;
                }
                while (parent_of_ancestor);
            }
            text_box_focus = inherited_focus;
        }
        widget_instance_render_text_box(widget, definition, clip_rect, child_offset, text_box_focus);
        break;
    }

    case _ui_widget_type_spinner_list:
        widget_instance_render_spinner_list(widget, definition, clip_rect, child_offset, focus);
        if ((definition->list_flags & (1u << _list_items_generated_from_string_list_tag)) != 0 && !definition->child_widget_references.count)
            should_recurse_children = 0;
        break;

    case _ui_widget_type_column_list:
        widget_instance_render_column_list(widget, definition, clip_rect, child_offset, focus);
        should_recurse_children = (definition->list_flags & (1u << _list_items_generated_in_code)) == 0;
        break;

    default:
        break;
    }

    if (should_recurse_children)
    {
        for (widget_instance *child = widget->children; child; child = child->next)
        {
            uint8_t child_focus = 0;
            if (widget->focused_child == child)
            {
                int widget_type = widget->widget_type;
                if (widget_type == _ui_widget_type_spinner_list || widget_type == _ui_widget_type_column_list)
                    child_focus = 1;
            }
            widget_instance_render_recursive(child, clip_rect, child_offset, widget->focused_child == child,
                                             child_focus);
        }
    }

    for (int i = 0; i < definition->event_handler_references.count; i++)
    {
        event_handler_reference *handler =
            (event_handler_reference *)definition->event_handler_references.address + i;
        if (handler->event_type == _widget_event_post_render)
        {
            /* the decompiler builds this via two raw dword writes into a reused rectangle2d-typed stack
             * slot (`*(_DWORD*)&v38.y1 = 0; *(_DWORD*)v38.n = local_player_index;`); reinterpreted here in
             * event_record terms: value union zeroed first, then type/controller_index dword set so that
             * type=0 and controller_index=widget->local_player_index (big-endian dword store). */
            event_record frame_event;
            frame_event.type = 0;
            frame_event.controller_index = widget->local_player_index;
            frame_event.value.value = 0;
            unsigned char deleted;
            event_handler_dispatch(widget, definition, &frame_event, handler, &deleted);
        }
    }
}
