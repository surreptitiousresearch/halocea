/* widget_instance_initialize @ 0x837357F8 — populate a freshly allocated widget_instance from
 * its definition: type/flags/timers/colour, load its children recursively, fire any
 * initialization (event_type 24) handlers, pick an initial focused child, and take a pause if
 * the widget requests one.
 *
 * Decompiler param naming was off: the 5th arg is the definition data (ui_widget_definition*),
 * the 6th (back-inhibit flag from the loader) is unused here. */

#include <stdint.h>
#include <string.h>
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/ui_widget_definition_flags.h"
#include "headers/widget_event_flags.h"
#include "headers/ui_widget_type.h"
#include "headers/event_handler_reference.h"
#include "headers/event_record.h"
#include "headers/global_tag_instances.h"
#include "headers/game_connection.h"

extern int ui_widget_load_children_recursive(widget_instance *widget, ui_widget_definition *widget_definition);
extern void event_handler_dispatch(widget_instance *widget, ui_widget_definition *definition, event_record *event, event_handler_reference *event_handler, unsigned char *calling_widget_deleted);
extern void widget_instance_give_focus_directly(widget_instance *widget, widget_instance *target);
extern int16_t game_connection(void);
extern uint8_t game_time_get_paused(void);
extern void game_time_set_paused(uint8_t paused);
extern void sound_pause(uint8_t paused);

void widget_instance_initialize(widget_instance *widget, widget_instance *parent,
                                int ui_widget_definition_index, int16_t local_player_index,
                                ui_widget_definition *definition, int back_inhibited)
{
    int16_t type;
    int background_index;
    int i;
    event_record init_event;
    unsigned char deleted[8];

    (void)back_inhibited;
    memset(widget, 0, sizeof(widget_instance));

    /* a text-box list item shares its parent's tag */
    if ( (definition->list_flags & (1u << _list_items_generated_from_string_list_tag)) && parent && ui_widget_definition_index == parent->ui_widget_tag_index )
        widget->widget_type = _ui_widget_type_text_box;

    widget->ui_widget_tag_index = ui_widget_definition_index;
    widget->local_player_index = local_player_index;
    widget->name = definition->name;
    type = definition->type;
    widget->widget_type = definition->type;
    widget->visible = 1;
    widget->render_regardless_of_controller_index =
        (definition->flags & (1u << _ui_widget_definition_render_regardless_of_controller_index_bit)) != 0;
    widget->pause_game_time = (definition->flags & (1u << _ui_widget_definition_pause_game_time_bit)) != 0;
    widget->creation_time = widget_globals.current_system_milliseconds;
    widget->milliseconds_to_auto_close =
        (definition->millisecond_to_auto_close > 0) ? definition->millisecond_to_auto_close : 0;
    widget->alpha_modifier = 1.0f;
    widget->parent = parent;
    widget->auto_close_fade_time =
        (definition->millisecond_auto_close_fade_time > 0) ? definition->millisecond_auto_close_fade_time : 0;

    if ( type == _ui_widget_type_text_box )
    {
        widget->parameters.text_box_parameters.string_list_index = -1;
        widget->parameters.text_box_parameters.text_color.n[0] = 0.0f;
    }

    /* number_of_sprite_frames comes from the background bitmap's first sequence */
    background_index = definition->background_bitmap.index;
    if ( background_index != -1 )
        /* sprite-frame count = bitmap_count of the background bitmap's first sequence */
        widget->animation_data.number_of_sprite_frames =
            ((bitmap_group_sequence *)(TAG_GET(bitmap_group, background_index))->sequences.address)[0].bitmap_count;

    if ( !widget_globals.dont_load_children_recursive )
        ui_widget_load_children_recursive(widget, definition);

    /* fire on-create (_widget_event_created) handlers */
    for ( i = 0; i < definition->event_handler_references.count; ++i )
    {
        event_handler_reference *handler =
            (event_handler_reference *)definition->event_handler_references.address + i;
        if ( handler->event_type == _widget_event_created )
        {
            init_event.type = 0;
            init_event.value.value = 0;
            init_event.controller_index = widget->local_player_index;
            event_handler_dispatch(widget, definition, &init_event, handler, deleted);
        }
    }

    /* pick the first focusable child if none was set */
    if ( !widget->focused_child )
    {
        widget_instance *child;
        for ( child = widget->children; child; child = child->next )
        {
            if ( !child->never_receive_events )
            {
                ui_widget_definition *child_def = TAG_GET(ui_widget_definition, child->ui_widget_tag_index);
                int wt;
                if ( child_def->event_handler_references.count > 0
                  || (wt = child->widget_type, wt == _ui_widget_type_spinner_list)
                  || wt == _ui_widget_type_column_list )
                {
                    widget_instance_give_focus_directly(widget, child);
                }
            }
        }
    }

    if ( widget->pause_game_time == 1 && game_connection() != _game_connection_network_server && !we_are_at_the_main_menu )
    {
        ++widget_globals.pause_game_time_count;
        if ( !game_time_get_paused() )
            game_time_set_paused(1u);
        if ( !widget_globals.sound_paused && !we_are_at_the_main_menu )
        {
            sound_pause(1u);
            widget_globals.sound_paused = 1;
        }
    }
}
