/* single_preview_column_list_update @0x83780BF8 — per-frame update for the generic single-preview-column
 * list widget (gametype/map/profile pickers). If the widget's first grandchild is a text box (widget_type
 * 2) it is the bank spinner: its string_list_index names one of the 3 preview_list_array_bank banks, and
 * tabbing it searches forward/backward (wrapping) for the next non-empty bank; entering a new bank resets
 * the selection to the bank's default item, drops focus, and forces a hot update. The row walk then
 * refreshes every child row: row 0 is the bank spinner (focus handoff to its own child), the first and
 * last data rows double as scroll-up/scroll-down arrow rows (their 3rd/4th grandchildren are the arrow
 * text boxes) unless the definition's no-arrows flag (+336 & 8) or a near-full list suppresses them, and
 * every ordinary row reallocs its 0x80-byte text and fills it through `func` (row grayed to 1/3 alpha on
 * failure). A trailing pass re-homes focus onto the first event-receiving row, or bounces focus to the
 * widget's own next sibling when nothing is focusable.
 *
 * DEVIATION: the decompiler emitted used-before-assigned v20/v22/v23 in the bank-switch block; disasm
 * 0x83780E08-0x83780E38 resolves them (r4 = the bank spinner text box, r5 = &preview_list_array_bank,
 * r6 = the new bank index) — the stores are spinner->string_list_index = bank and widget number_of_items
 * = preview_list_array_bank[bank].count.
 * DEVIATION: the forward bank-scan's end sentinel `(int)p_count >= (int)&build_number_string[2]` decodes
 * to &preview_list_array_bank[3].count (bank array @0x8446A50C, build_number_string @0x8446A530 = +36 =
 * array end; [2] wide chars = +4 = the .count offset) — written as the bank index bound.
 * Faithful shipped quirk: on the selected row both branches join at 0x837811DC which stores
 * children->next->visible = 0, immediately overwriting the visible = 1 written for the selection
 * highlight at 0x83781138 — the highlight grandchild can never actually show. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/dynamic_array.h"
#include "headers/real_argb_color.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_definition.h"
#include "headers/ui_widget_type.h"
#include "headers/ui_widget_list_flags.h"
#include "headers/blam_data_globals.h"


extern int preview_list_get_default_item_for_bank(int bank);
extern int spcl_ensure_top_item_visible(widget_instance *widget);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern real_argb_color * get_ui_argb_white(real_argb_color *result);

void single_preview_column_list_update(widget_instance *widget,
        uint8_t (__fastcall *func)(uint16_t *, int, void *))
{
    int list_item_top_index = widget->parameters.list_parameters.list_item_top_index;
    widget_instance *children = widget->children;
    ui_widget_definition *definition = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);
    int visible_count = definition->child_widget_references.count;

    uint8_t has_bank_spinner = children && children->children
            && children->children->widget_type == _ui_widget_type_spinner_list;
    uint8_t focus_is_live = widget->focused_child && widget->focused_child->next != nullptr;

    unsigned int bank = 0;
    int tab_forward = 0;
    widget_instance *bank_spinner = widget->children;
    if ( has_bank_spinner )
    {
        bank_spinner = children->children;
        bank = bank_spinner->parameters.text_box_parameters.string_list_index;
        tab_forward = bank_spinner->parameters.list_parameters.last_list_tab_direction > 0;
    }

    if ( bank != preview_list_current_bank )
    {
        if ( preview_list_current_bank == -1 )
        {
            if ( !preview_list_array_bank[bank].count )
            {
                /* first non-empty bank at or after this one (sentinel = array end, see header) */
                while ( ++bank < 3 )
                {
                    if ( preview_list_array_bank[bank].count )
                        goto bank_scanned;
                }
                bank = 0;
                goto switch_bank;
            }
        }
        else if ( tab_forward )
        {
            if ( !preview_list_array_bank[bank].count )
            {
                while ( bank != preview_list_current_bank )
                {
                    /* branchless wrap: idx+1, or 0 when idx+1 reaches 3 */
                    bank = -(((bank + 1) >> 31) + (bank + 1 < 3)) & (bank + 1);
                    if ( preview_list_array_bank[bank].count )
                        goto bank_scanned;
                }
                goto stay_in_bank;
            }
        }
        else if ( !preview_list_array_bank[bank].count )
        {
            while ( bank != preview_list_current_bank )
            {
                if ( (int)--bank < 0 )
                    bank = 2;
                if ( preview_list_array_bank[bank].count )
                    goto bank_scanned;
            }
            goto stay_in_bank;
        }

bank_scanned:
        if ( bank != preview_list_current_bank )
        {
switch_bank:
            {
                int default_item = preview_list_get_default_item_for_bank(bank);
                bank_spinner->parameters.text_box_parameters.string_list_index = bank;
                widget->parameters.list_parameters.list_item_top_index = 0;
                widget->parameters.text_box_parameters.string_list_index = 0;
                widget->parameters.list_parameters.selected_list_item_index = 0;
                widget->parameters.list_parameters.number_of_items = preview_list_array_bank[bank].count;
                if ( default_item == -1 )
                {
                    list_item_top_index = 0;
                }
                else
                {
                    list_item_top_index = -1;
                    widget->parameters.text_box_parameters.string_list_index = default_item;
                    widget->parameters.list_parameters.selected_list_item_index = default_item;
                }
                widget->focused_child = nullptr;
                preview_list_current_bank = bank;
                should_force_hot_update = 1;
                goto bank_resolved;
            }
        }
stay_in_bank:
        bank_spinner->parameters.text_box_parameters.string_list_index = bank;
        widget->parameters.list_parameters.number_of_items = preview_list_array_bank[bank].count;
    }

bank_resolved:
    if ( has_bank_spinner )
        --visible_count;

    uint8_t near_full = (definition->list_flags & (1u << _list_single_preview_box_no_scroll)) != 0
            || widget->parameters.list_parameters.number_of_items <= visible_count - 1;

    int window = visible_count - (near_full ? 1 : 3);
    if ( window > widget->parameters.list_parameters.number_of_items )
        window = widget->parameters.list_parameters.number_of_items;

    if ( list_item_top_index == -1 )
        list_item_top_index = spcl_ensure_top_item_visible(widget);

    widget_instance *row = widget->children;
    int item_index = list_item_top_index;
    int row_number = 0;
    while ( row )
    {
        widget_instance *next = row->next;
        if ( !next )
            break;
        row->visible = 1;
        if ( item_index >= widget->parameters.list_parameters.number_of_items )
        {
            row->visible = 0;
            goto disable_row;
        }
        if ( row_number )
        {
            if ( row_number != 1 || near_full || !has_bank_spinner )
                goto data_row;
        }
        else
        {
            if ( has_bank_spinner )
            {
                /* row 0 = the bank spinner: hand focus down to its own child */
                if ( widget->focused_child == row )
                {
                    widget_instance *spinner_child = row->children;
                    --item_index;
                    row->animation_data.current_frame_index = 1;
                    row->focused_child = spinner_child;
                }
                else
                {
                    row->animation_data.current_frame_index = 0;
                    --item_index;
                    row->focused_child = nullptr;
                }
                goto next_row;
            }
            if ( near_full )
                goto data_row;
        }

        /* scroll-up arrow row (3rd grandchild is the arrow) */
        row->children->visible = 0;
        row->animation_data.current_frame_index = 0;
        row->children->next->visible = 0;
        row->children->next->next->next->visible = 0;
        row->children->next->next->visible = 1;
        row->children->next->next->animation_data.current_frame_index = widget->focused_child == row;
        if ( list_item_top_index )
        {
            row->alpha_modifier = 1.0f;
            row->never_receive_events = 0;
            --item_index;
            goto next_row;
        }
        row->alpha_modifier = 0.333f;
        row->never_receive_events = 1;
        if ( widget->focused_child != row )
        {
            --item_index;
            goto next_row;
        }
        --item_index;
        widget->focused_child = row->next;
        goto next_row;

data_row:
        if ( !next->next && !near_full )
        {
            /* scroll-down arrow row (4th grandchild is the arrow) */
            row->children->visible = 0;
            row->animation_data.current_frame_index = 0;
            row->children->next->visible = 0;
            row->children->next->next->visible = 0;
            row->children->next->next->next->visible = 1;
            row->children->next->next->next->animation_data.current_frame_index =
                    widget->focused_child == row;
            if ( list_item_top_index >= widget->parameters.list_parameters.number_of_items - window )
            {
                row->alpha_modifier = 0.333f;
                row->never_receive_events = 1;
                if ( widget->focused_child == row )
                    widget->focused_child = row->previous;
                goto next_row;
            }
enable_row:
            row->alpha_modifier = 1.0f;
            row->never_receive_events = 0;
            goto next_row;
        }

        /* ordinary item row */
        row->children->visible = 1;
        row->children->next->next->visible = 0;
        row->children->next->next->next->visible = 0;
        if ( item_index == widget->parameters.list_parameters.selected_list_item_index )
        {
            row->children->next->visible = 1; /* dead store — overwritten below (shipped quirk) */
            row->animation_data.current_frame_index = 1;
            if ( widget->focused_child == row )
            {
                real_argb_color white;
                row->children->parameters.text_box_parameters.text_color = *get_ui_argb_white(&white);
            }
            else
            {
                row->children->parameters.text_box_parameters.text_color.n[0] = 0.0f;
            }
        }
        else
        {
            if ( widget->focused_child == row )
            {
                widget->parameters.text_box_parameters.string_list_index = item_index;
                real_argb_color white;
                row->children->parameters.text_box_parameters.text_color = *get_ui_argb_white(&white);
            }
            else
            {
                row->children->parameters.text_box_parameters.text_color.n[0] = 0.0f;
            }
            row->animation_data.current_frame_index = 0;
        }
        row->children->next->visible = 0;

        {
            widget_instance *label = row->children;
            uint16_t *text = ui_widget_realloc(
                    label->parameters.text_box_parameters.text, 0x80u);
            label->parameters.text_box_parameters.text = text;
            if ( !text || !func(text, item_index, widget->parameters.list_parameters.list_items) )
            {
                row->alpha_modifier = 0.333f;
disable_row:
                row->never_receive_events = 1;
                goto next_row;
            }
        }
        if ( focus_is_live || item_index == widget->parameters.list_parameters.selected_list_item_index )
            goto enable_row;
        row->alpha_modifier = 0.333f;
        row->never_receive_events = 0;

next_row:
        row = row->next;
        ++row_number;
        ++item_index;
    }

    /* re-home focus onto the first event-receiving row */
    widget_instance *focused = widget->focused_child;
    if ( !focused || focused->never_receive_events )
    {
        widget_instance *candidate = widget->children;
        widget->focused_child = nullptr;
        if ( candidate )
        {
            while ( candidate->never_receive_events )
            {
                candidate = candidate->next;
                if ( !candidate )
                    goto no_focusable_row;
            }
            widget->focused_child = candidate;
        }
no_focusable_row:
        if ( !widget->focused_child )
            widget->parent->focused_child = widget->next;
    }
}
