/* ui_widget_delete @ 0x83732D08 — tear down a widget and its whole subtree: fire any
 * on-close (event_type 25) handlers, undo a pause this widget owned, recursively delete
 * children, unlink from its sibling/parent lists, free its text/list payload and extended
 * description, return its memory to the pool, and clear it from the active-widget slots.
 * Guarded against re-entrancy by delete_recursion_lock. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/event_handler_reference.h"
#include "headers/global_tag_instances.h"
#include "headers/game_connection.h"
#include "headers/event_handler_flags.h"
#include "headers/ui_widget_type.h"
#include "headers/widget_event_flags.h"

typedef struct event_record event_record;
#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
#include "headers/widget_deleted_player_control_flags.h"
extern void player_control_inhibit_buttons(int16_t local_player_index, uint16_t button_flags, uint8_t uninhibit_when_released);
extern uint8_t ui_widget_event_handler_function_invoke(widget_instance *widget, event_record *event, uint16_t function, unsigned char *widget_deleted);
extern widget_instance *ui_widget_launch_widget(widget_instance *widget, int new_widget_tag_index);
extern int16_t game_connection(void);
extern uint8_t game_time_get_paused(void);
extern void game_time_set_paused(uint8_t paused);
extern void sound_pause(uint8_t paused);
extern void main_menu_ensure_player_queues_exist(void);
extern void game_time_dispose_from_old_map(void);
extern void game_time_initialize_for_new_map(void);
extern void game_time_start(void);
extern void dispose_pointer(stack_memory_pool *pool, void *p);

void ui_widget_delete(widget_instance *widget)
{
    int local_player_index;
    ui_widget_definition *definition;
    widget_instance *child;
    widget_instance *previous, *next, *parent;
    unsigned int widget_type;
    int i;

    if ( widget->delete_recursion_lock )
        return;

    local_player_index = (uint16_t)widget->local_player_index;
    widget->delete_recursion_lock = 1;

    if ( local_player_index != 0xFFFF && !widget->parent )
        player_control_inhibit_buttons(local_player_index, WIDGET_DELETED_PLAYER_CONTROL_INHIBIT_FLAGS, 1u);

    /* run on-close handlers */
    definition = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);
    for ( i = 0; i < definition->event_handler_references.count; ++i )
    {
        event_handler_reference *handler =
            (event_handler_reference *)definition->event_handler_references.address + i;
        if ( handler->event_type == _widget_event_deleted && (handler->flags & (1u << _event_handler_run_function_bit)) )
        {
            unsigned char consumed = 0;
            if ( ui_widget_event_handler_function_invoke(widget, 0, handler->function, &consumed) == 1
              && (handler->flags & (1u << _event_handler_open_widget_bit)) )
            {
                int launch_tag = handler->ui_widget_tag.index;
                if ( launch_tag != -1 )
                    ui_widget_launch_widget(widget, launch_tag);
            }
        }
    }

    /* release a pause this widget had taken */
    if ( widget->pause_game_time == 1
      && game_connection() != _game_connection_network_server
      && !we_are_at_the_main_menu
      && !--widget_globals.pause_game_time_count )
    {
        if ( game_time_get_paused() )
        {
            game_time_set_paused(0);
            if ( we_are_at_the_main_menu )
            {
                main_menu_ensure_player_queues_exist();
                game_time_dispose_from_old_map();
                game_time_initialize_for_new_map();
                game_time_start();
            }
        }
        if ( widget_globals.sound_paused == 1 )
        {
            sound_pause(0);
            widget_globals.sound_paused = 0;
        }
    }

    /* recursively delete children */
    child = widget->children;
    while ( child )
    {
        next = child->next;
        ui_widget_delete(child);
        if ( next )
            next->previous = 0;
        child = next;
    }

    /* unlink from sibling list / parent */
    previous = widget->previous;
    if ( previous )
        previous->next = widget->next;
    next = widget->next;
    if ( next )
        next->previous = widget->previous;
    parent = widget->parent;
    if ( parent && parent->children == widget )
        parent->children = widget->next;

    /* free type-specific payload */
    widget_type = (uint16_t)widget->widget_type;
    if ( widget_type == _ui_widget_type_text_box )
    {
        uint16_t *text = widget->parameters.text_box_parameters.text;
        if ( text )
            dispose_pointer(widget_memory_pool, text);
    }
    else if ( widget_type > _ui_widget_type_text_box && widget_type <= _ui_widget_type_column_list )
    {
        uint16_t *item_text = widget->parameters.list_parameters.item_text;
        widget_instance *extended_description;
        if ( item_text )
            dispose_pointer(widget_memory_pool, item_text);
        extended_description = widget->parameters.list_parameters.extended_description;
        if ( extended_description )
            ui_widget_delete(extended_description);
    }

    dispose_pointer(widget_memory_pool, widget);

    /* clear from the active-widget slots */
    /* recovered: slot pointer-walk -> active_widgets[2] index loop */
    {
        int slot_index;
        for ( slot_index = 0; slot_index < 2; ++slot_index )
        {
            if ( widget_globals.active_widgets[slot_index] == widget )
            {
                widget_globals.active_widgets[slot_index] = 0;
                break;
            }
        }
    }
}
