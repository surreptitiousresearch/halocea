/* single_preview_column_list_item_activated @0x83697C60 — event handler for the activatable rows of a
 * "single preview column" list widget. On a valid activation (a button-press event whose button is the
 * accept/back pair {0,12}, at least 250 ms after the widget was created) it walks the parent list's child
 * widgets to find which visible row this widget is, tracking the corresponding item index. If the activated
 * row is the first/last visible row of a scrollable list it scrolls the list (adjusting list_item_top_index)
 * and plays the scroll sound; otherwise it selects the row's item and, when the same item was already
 * selected on a genuine activation, queues a custom activation event. Always returns 1 (handled).
 *
 * The list widget tag definition supplies the row-slot count (@+992) and a "no-scroll" flag bit (@+336 & 8);
 * a leading spinner child (has_spinner_as_first_child) shifts the slot count and item mapping by one. The
 * two goto targets of the original are preserved to keep the branch structure exact. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_definition.h"
#include "headers/event_type.h"
#include "headers/gamepad_button.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/ui_widget_list_flags.h"
#include "headers/standard_repeat_delay.h"

extern unsigned __int8 has_spinner_as_first_child(widget_instance *widget);
extern uint32_t system_milliseconds(void);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern void event_manager_queue_custom_activation_event(int16_t player_number);

int single_preview_column_list_item_activated(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    widget_instance *parent = widget->parent;
    widget_instance *child_iter = parent->children;
    __int16 list_item_top_index = parent->parameters.list_parameters.list_item_top_index;
    ui_widget_definition *list_widget_definition =
        TAG_GET(ui_widget_definition, parent->ui_widget_tag_index);
    int item_slot_count = list_widget_definition->child_widget_references.count;
    int has_spinner = has_spinner_as_first_child(parent);
    __int16 selected_list_item_index = parent->parameters.list_parameters.selected_list_item_index;

    /* an activation is a button press of the accept/back buttons, held for >250 ms since creation */
    char activated = event->type == event_type_button
        && (event->value.button_value.button == _gamepad_analog_button_a || event->value.button_value.button == _gamepad_binary_button_start)
        && system_milliseconds() - widget->creation_time > STANDARD_REPEAT_DELAY_MILLISECONDS;

    if ( has_spinner )
        --item_slot_count;

    int no_scroll = ((list_widget_definition->list_flags & (1u << _list_single_preview_box_no_scroll)) != 0
                     || parent->parameters.list_parameters.number_of_items <= item_slot_count - 1) ? 1 : 0;

    int number_of_items = parent->parameters.list_parameters.number_of_items;
    int visible_rows = item_slot_count - (no_scroll ? 1 : 3);
    if ( visible_rows > number_of_items )
        visible_rows = parent->parameters.list_parameters.number_of_items;

    int running_item_index = list_item_top_index;
    int child_position = 0;

    while ( child_iter )
    {
        widget_instance *next_child = child_iter->next;
        if ( !next_child )
            return 1;

        if ( widget == child_iter )
        {
            if ( child_position )
            {
                if ( child_position != 1 || !has_spinner )
                    goto scroll_from_last;
            }
            else if ( has_spinner )
            {
scroll_from_last:
                if ( !next_child->next && !no_scroll )
                {
                    /* LOWORD store into the __int16 top-index field */
                    int new_top = visible_rows + list_item_top_index - 1;
                    if ( new_top >= number_of_items - visible_rows )
                        new_top = number_of_items - visible_rows;
                    parent->parameters.list_parameters.list_item_top_index = (__int16)new_top;
                    ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
                    return 1;
                }
                goto select_item;
            }

            if ( !no_scroll )
            {
                /* DEVIATION: original clamps via the branchless ((x>>31)-1)&x == max(0, x) */
                int candidate = list_item_top_index - visible_rows + 1;
                parent->parameters.list_parameters.list_item_top_index =
                    (__int16)(candidate < 0 ? 0 : candidate);
                ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
                return 1;
            }

select_item:
            ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
            parent->parameters.list_parameters.selected_list_item_index = (__int16)running_item_index;
            if ( activated && selected_list_item_index == running_item_index )
                event_manager_queue_custom_activation_event(0);
            return 1;
        }

        /* advance the running item index past a non-activated child */
        if ( child_position )
        {
            if ( child_position == 1 && !no_scroll && has_spinner )
                --running_item_index;
        }
        else if ( has_spinner || !no_scroll )
        {
            --running_item_index;
        }
        ++child_position;
        child_iter = child_iter->next;
        ++running_item_index;
    }

    return 1;
}
