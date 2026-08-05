/* widget_instance_process_one_event_recursive @ 0x83735AB0 — deliver one input event to a
 * widget and (when the event is not consumed) recurse into its children / focused child.
 * Handles: auto-close-on-controller, default back/cancel, auto-close timeout + fade, list and
 * tab navigation driven by the definition's flag bits, the widget's own event-handler table,
 * "return to main menu" when the last widget closes, and the UI feedback sound. Sets
 * *return_widget_deleted if the widget (or an ancestor) was deleted while processing.
 *
 * The flag-driven navigation blocks are reproduced with labels mirroring the decompiler to
 * preserve the exact button/axis → action mapping. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/event_record.h"
#include "headers/event_handler_reference.h"
#include "headers/global_tag_instances.h"
#include "headers/event_type.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/ui_widget_type.h"
#include "headers/widget_event_flags.h"

extern void ui_widget_delete(widget_instance *widget);
extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern void widget_instance_go_back_to_previous(widget_instance *widget);
extern void widget_instance_tab_to_next_valid_widget(widget_instance *widget);
extern void widget_instance_tab_to_previous_valid_widget(widget_instance *widget);
extern uint8_t widget_event_function_list_widget_goto_next_item(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t widget_event_function_list_widget_goto_previous_item(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern void event_handler_dispatch(widget_instance *widget, ui_widget_definition *definition, event_record *event, event_handler_reference *event_handler, unsigned char *calling_widget_deleted);
extern void main_goto_main_menu(void);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);

extern void widget_instance_update_animation_parameters(widget_instance *widget, ui_widget_definition *definition);
void widget_instance_process_one_event_recursive(widget_instance *widget,
                                                 ui_widget_definition *definition,
                                                 event_record *event,
                                                 uint8_t *return_widget_deleted)
{
    unsigned char deleted[8];
    char event_consumed = 0;
    char applies_to_player;
    int sound_code = 0;
    unsigned char nav_blocked;
    unsigned int flags;

    deleted[0] = 0;

    /* does this event target this widget? */
    if ( widget->never_receive_events )
    {
        applies_to_player = 0;
    }
    else
    {
        int lpi = widget->local_player_index;
        applies_to_player = (lpi == -1 || lpi == event->controller_index) ? 1 : 0;
    }

    /* auto-close this dialog as soon as the relevant controller is present */
    if ( widget->close_if_local_player_controller_present == 1 )
    {
        int16_t lpi = widget->local_player_index;
        int present;
        if ( (unsigned int)lpi >= 4 )
        {
            int c;
            present = 0;
            for ( c = 0; c < 4; ++c )
            {
                if ( input_has_gamepad(c) )
                {
                    present = 1;
                    break;
                }
            }
        }
        else
        {
            present = input_has_gamepad(lpi);
        }
        if ( present )
        {
            widget_instance *root = widget, *p;
            for ( p = widget->parent; p; p = p->parent )
                root = p;
            ui_widget_delete(root);
            deleted[0] = 1;
        }
    }

    /* default back/cancel: a B (1) or Back (13) press with no explicit handler bound */
    if ( applies_to_player && !deleted[0] && event->type == event_type_button && event->value.button_value.value == 1 )
    {
        int button = event->value.button_value.button;
        char do_default_back = 0;

        if ( button == _widget_event_button_back || button == _widget_event_button_b )
        {
            int count = definition->event_handler_references.count;
            int i;
            do_default_back = 1;
            for ( i = 0; i < count; ++i )
            {
                event_handler_reference *h =
                    (event_handler_reference *)definition->event_handler_references.address + i;
                if ( h->event_type == button )
                {
                    do_default_back = 0;
                    break;
                }
            }
        }
        if ( do_default_back )
        {
            widget_instance_go_back_to_previous(widget);
            deleted[0] = 1;
            sound_code = _ui_audio_feedback_back;
            event_consumed = 1;
        }
    }

    /* auto-close timeout / fade */
    {
        unsigned int ms_to_close = widget->milliseconds_to_auto_close;
        if ( ms_to_close == 1 )
            widget->visible = 0;
        if ( !deleted[0] && ms_to_close )
        {
            unsigned int fade_time = widget->auto_close_fade_time;
            unsigned int age = widget_globals.current_system_milliseconds - widget->creation_time;
            if ( age < fade_time + ms_to_close )
            {
                if ( fade_time )
                {
                    int past_close = (int)(age - ms_to_close);
                    if ( past_close > 0 )
                    {
                        widget_instance *root = widget, *p;
                        for ( p = widget->parent; p; p = p->parent )
                            root = p;
                        root->alpha_modifier = 1.0f - (float)past_close / (float)fade_time;
                    }
                }
            }
            else
            {
                widget_instance *root = widget, *p;
                for ( p = widget->parent; p; p = p->parent )
                    root = p;
                ui_widget_delete(root);
                deleted[0] = 1;
            }
        }
    }

    if ( !deleted[0] )
    {
        int16_t widget_type = widget->widget_type;

        /* clamp negative animation frame indices to 0 */
        /* DEVIATION: verbatim inlined copy of widget_instance_update_animation_parameters@0x837320F0 (donor's `definition` param is unused in its body, so it is passed through unmodified rather than folded) — collapsed to a call */
        widget_instance_update_animation_parameters(widget, definition);

        /* refresh focus-highlight sprite frames on list / column-list containers */
        if ( widget_type == _ui_widget_type_spinner_list )
        {
            widget_instance *child;
            if ( definition->child_widget_references.count == 3 && !widget->focused_child )
                widget->focused_child = widget->children;
            for ( child = widget->children; child; child = child->next )
            {
                child->animation_data.current_frame_index = 0;
                if ( child == widget->focused_child && child->animation_data.number_of_sprite_frames == 2 )
                    child->animation_data.current_frame_index = 1;
            }
        }
        else if ( widget_type == _ui_widget_type_column_list )
        {
            widget_instance *child;
            for ( child = widget->children; child; child = child->next )
            {
                int sprites = (uint16_t)child->animation_data.number_of_sprite_frames;
                if ( child == widget->focused_child )
                {
                    if ( sprites == 2 )
                        child->animation_data.current_frame_index = 1;
                }
                else if ( sprites == 2 )
                {
                    child->animation_data.current_frame_index = 0;
                }
            }
        }

        /* --- _widget_dpad_updown_tabs_thru_children_bit: tab between children (dpad up/down or right-stick Y) --- */
        if ( applies_to_player && !event_consumed && (definition->flags & (1u << _widget_dpad_updown_tabs_thru_children_bit)) && widget->focused_child )
        {
            nav_blocked = deleted[0];
            if ( !deleted[0] )
            {
                if ( event->type == event_type_button && event->value.button_value.value == 1 )
                {
                    int b = event->value.button_value.button;
                    if ( b == _widget_event_button_dpad_down )
                        goto tab8_next;
                    if ( b == _widget_event_button_dpad_up )
                        goto tab8_prev;
                }
                else if ( event->type == event_type_left_analog_stick )
                {
                    if ( event->value.analog_value.n[1] == -32768 )
                        goto tab8_next;
                    if ( event->value.analog_value.n[1] == 0x7FFF )
                        goto tab8_prev;
                }
                goto after_tab8;
            tab8_next:
                widget_instance_tab_to_next_valid_widget(widget);
                if ( !sound_code ) sound_code = _ui_audio_feedback_cursor;
                event_consumed = 1;
                goto after_tab8;
            tab8_prev:
                widget_instance_tab_to_previous_valid_widget(widget);
                if ( !sound_code ) sound_code = _ui_audio_feedback_cursor;
                event_consumed = 1;
            }
        after_tab8: ;
        }
        else
        {
            nav_blocked = deleted[0];
        }

        if ( !applies_to_player )
            goto recurse_section;

        /* --- _widget_dpad_leftright_tabs_thru_children_bit: tab between children (dpad left/right or left-stick X) --- */
        if ( !(event_consumed || (definition->flags & (1u << _widget_dpad_leftright_tabs_thru_children_bit)) == 0 || !widget->focused_child || nav_blocked) )
        {
            if ( event->type == event_type_button && event->value.button_value.value == 1 )
            {
                int b = event->value.button_value.button;
                if ( b == _widget_event_button_dpad_right )
                    goto tab10_next;
                if ( b == _widget_event_button_dpad_left )
                    goto tab10_prev;
            }
            else if ( event->type == event_type_left_analog_stick )
            {
                if ( event->value.analog_value.n[0] == 0x7FFF )
                    goto tab10_next;
                if ( event->value.analog_value.n[0] == -32768 )
                    goto tab10_prev;
            }
            goto list20_section;
        tab10_next:
            widget_instance_tab_to_next_valid_widget(widget);
            if ( !sound_code ) sound_code = 1;
            event_consumed = 1;
            goto list20_section;
        tab10_prev:
            widget_instance_tab_to_previous_valid_widget(widget);
            if ( !sound_code ) sound_code = 1;
            event_consumed = 1;
        }

    list20_section:
        /* --- _widget_dpad_updown_tabs_thru_list_items_bit: list item step (dpad up/down or right-stick Y) --- */
        if ( (definition->flags & (1u << _widget_dpad_updown_tabs_thru_list_items_bit))
          && (widget->widget_type == _ui_widget_type_spinner_list || widget->widget_type == _ui_widget_type_column_list)
          && !event_consumed && !nav_blocked )
        {
            if ( event->type == event_type_button && event->value.button_value.value == 1 )
            {
                int b = event->value.button_value.button;
                if ( b == 9 )
                    goto list20_next;
                if ( b == 8 )
                    goto list20_prev;
            }
            else if ( event->type == event_type_left_analog_stick )
            {
                if ( event->value.analog_value.n[1] == -32768 )
                    goto list20_next;
                if ( event->value.analog_value.n[1] == 0x7FFF )
                    goto list20_prev;
            }
            goto list40_section;
        list20_next:
            widget_event_function_list_widget_goto_next_item(widget, event, deleted);
            if ( !sound_code ) sound_code = 1;
            event_consumed = 1;
            goto list40_section;
        list20_prev:
            widget_event_function_list_widget_goto_previous_item(widget, event, deleted);
            if ( !sound_code ) sound_code = 1;
            event_consumed = 1;
        }

    list40_section:
        /* --- _widget_dpad_leftright_tabs_thru_list_items_bit: list item step (dpad left/right or left-stick X) --- */
        if ( (definition->flags & (1u << _widget_dpad_leftright_tabs_thru_list_items_bit))
          && (widget->widget_type == _ui_widget_type_spinner_list || widget->widget_type == _ui_widget_type_column_list)
          && !event_consumed && !deleted[0] )
        {
            if ( event->type == event_type_button && event->value.button_value.value == 1 )
            {
                int b = event->value.button_value.button;
                if ( b == _widget_event_button_dpad_right )
                    goto list40_next;
                if ( b == _widget_event_button_dpad_left )
                    goto list40_prev;
            }
            else if ( event->type == event_type_left_analog_stick )
            {
                if ( event->value.analog_value.n[0] == 0x7FFF )
                    goto list40_next;
                if ( event->value.analog_value.n[0] == -32768 )
                    goto list40_prev;
            }
            goto handler_section;
        list40_next:
            widget_event_function_list_widget_goto_next_item(widget, event, deleted);
            if ( !sound_code ) sound_code = 1;
            event_consumed = 1;
            goto handler_section;
        list40_prev:
            widget_event_function_list_widget_goto_previous_item(widget, event, deleted);
            if ( !sound_code ) sound_code = 1;
            event_consumed = 1;
        }
    }

handler_section:
    /* run the widget's own event-handler table */
    if ( applies_to_player )
    {
        int count = definition->event_handler_references.count;
        int i;
        for ( i = 0; i < count; ++i )
        {
            event_handler_reference *handler;
            unsigned char matched = 0;
            unsigned char hit = 0;

            if ( deleted[0] )
                break;

            handler = (event_handler_reference *)definition->event_handler_references.address + i;
            if ( (unsigned int)((uint16_t)event->type - 1) <= 4 )
            {
                int16_t et = handler->event_type;
                switch ( event->type )
                {
                    case event_type_left_analog_stick:
                        if ( (unsigned int)(uint16_t)(et - _widget_event_left_analog_stick_up) <= 3 )
                        {
                            if ( et == _widget_event_left_analog_stick_down )      hit = ((uint16_t)event->value.analog_value.n[1] == 0x8000);
                            else if ( et == _widget_event_left_analog_stick_left ) hit = ((uint16_t)event->value.analog_value.n[0] == 0x8000);
                            else if ( et == _widget_event_left_analog_stick_up )   hit = (event->value.analog_value.n[1] == 0x7FFF);
                            else                 hit = (event->value.analog_value.n[0] == 0x7FFF);
                            matched = hit;
                        }
                        break;
                    case event_type_right_analog_stick:
                        if ( (unsigned int)(uint16_t)(et - _widget_event_right_analog_stick_up) <= 3 )
                        {
                            if ( et == _widget_event_right_analog_stick_up )        hit = (event->value.analog_value.n[1] == 0x7FFF);
                            else if ( et == _widget_event_right_analog_stick_down ) hit = ((uint16_t)event->value.analog_value.n[1] == 0x8000);
                            else if ( et == _widget_event_right_analog_stick_left ) hit = ((uint16_t)event->value.analog_value.n[0] == 0x8000);
                            else                 hit = (event->value.analog_value.n[0] == 0x7FFF);
                            matched = hit;
                        }
                        break;
                    case event_type_button:
                        if ( et == event->value.button_value.button )
                            matched = (event->value.button_value.value == 1);
                        break;
                    case event_type_mouse_button:
                        break;
                    default:
                        matched = (et == _widget_event_custom_activation);
                        break;
                }
            }

            if ( matched )
            {
                event_consumed = 1;
                event_handler_dispatch(widget, definition, event, handler, deleted);
            }
        }
    }

recurse_section:
    /* recurse into children / focused child unless the event was consumed/closed */
    flags = definition->flags;
    if ( ((flags & (1u << _widget_pass_handled_events_to_all_children_bit)) != 0 || !event_consumed)
      && ((flags & (1u << _widget_pass_unhandled_events_to_children_bit)) != 0 || (flags & (1u << _widget_pass_unhandled_events_to_all_children_bit)) != 0)
      && !deleted[0] )
    {
        if ( flags & (1u << _widget_pass_unhandled_events_to_all_children_bit) )
        {
            widget_instance *child;
            for ( child = widget->children; child; child = child->next )
            {
                int lpi = child->local_player_index;
                if ( lpi == -1 || lpi == event->controller_index )
                {
                    widget_instance_process_one_event_recursive(
                        child,
                        TAG_GET(ui_widget_definition, child->ui_widget_tag_index),
                        event, deleted);
                    if ( deleted[0] == 1 )
                        break;
                }
            }
        }
        else
        {
            widget_instance *focused = widget->focused_child;
            if ( focused )
            {
                int lpi = focused->local_player_index;
                if ( lpi == -1 || lpi == event->controller_index )
                    widget_instance_process_one_event_recursive(
                        focused,
                        TAG_GET(ui_widget_definition, focused->ui_widget_tag_index),
                        event, deleted);
            }
        }
    }

    /* if this widget closed and had _widget_return_to_main_menu_if_no_history_bit, and only the menu root remains, go to menu */
    if ( deleted[0] == 1 && (definition->flags & (1u << _widget_return_to_main_menu_if_no_history_bit)) )
    {
        /* recovered: slot pointer-walk -> active_widgets[2] index loop (count leading empty slots) */
        int index = 0;
        while ( index < 2 && !widget_globals.active_widgets[index] )
            ++index;
        if ( index == 2 )
            main_goto_main_menu();
    }

    /* play the UI feedback sound matching the action taken */
    if ( (int16_t)sound_code >= _ui_audio_feedback_cursor
      && (int16_t)sound_code <= _ui_audio_feedback_flag_failure )
    {
        const char *sound_name;
        int sound_tag;
        switch ( (int16_t)sound_code )
        {
            case _ui_audio_feedback_forward:  sound_name = "sound\\sfx\\ui\\forward"; break;
            case _ui_audio_feedback_back:  sound_name = "sound\\sfx\\ui\\back"; break;
            case _ui_audio_feedback_cursor:  sound_name = "sound\\sfx\\ui\\cursor"; break;
            default: /* _ui_audio_feedback_flag_failure */ sound_name = "sound\\sfx\\ui\\flag_failure"; break;
        }
        sound_tag = tag_loaded(0x736E6421u /* 'snd!' */, sound_name);
        if ( sound_tag != -1 )
            unspatialized_impulse_sound_new(sound_tag, 1.0f);
    }

    *return_widget_deleted = deleted[0];
}
