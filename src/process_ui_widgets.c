/* process_ui_widgets @ 0x83737030 — per-frame UI widget pump. Finishes async
 * initialization (filesystem check thread), then, unless a progress bar / virtual
 * keyboard / attract mode pre-empts it, dispatches input events to the two active
 * widget trees and pops finished widgets off the stack. Pending deferred errors
 * (dashboard, generic, cinematic) are surfaced as error dialogs when appropriate. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/event_record.h"
#include "headers/file_system_check_results.h"
#include "headers/error_type.h"
#include "headers/global_tag_instances.h"

/* the widget's tag definition is treated as opaque here — only passed through */
typedef struct ui_widget_definition ui_widget_definition;

#include "headers/thread.h"
#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern uint32_t system_milliseconds(void);
extern uint8_t thread_has_exited(const thread *thread_reference);
extern void dispose_thread(thread *thread_reference);
extern uint8_t bink_playback_in_progress(void);
extern void bink_playback_stop(void);
extern void display_error(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern uint8_t progress_bar_is_active(void);
extern uint8_t virtual_keyboard_active(void);
extern void virtual_keyboard_process(void);
extern void event_manager_flush(void);
extern uint8_t attract_mode_should_start(void);
extern void attract_mode_start(void);
extern int ui_check_for_pause_game(void);
extern uint8_t get_next_event(event_record *event, int16_t local_player_index);
extern void widget_instance_process_one_event_recursive(widget_instance *widget, ui_widget_definition *definition, event_record *event, uint8_t *return_widget_deleted);
extern void dispose_pointer(stack_memory_pool *pool, void *p);
extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, int16_t local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, int16_t focused_child_index);
extern void widget_instance_set_focused_child_by_index(widget_instance *widget, int focused_child_parent_widget_tag, int16_t focused_child_index);
extern uint8_t network_game_is_active(void);
extern int game_time_get(void);

void process_ui_widgets(void)
{
    char events_dispatched = 0;

    widget_globals.current_system_milliseconds = system_milliseconds();

    if ( widget_globals.initialization_thread )
    {
        if ( thread_has_exited(widget_globals.initialization_thread) )
        {
            dispose_thread(widget_globals.initialization_thread);
            widget_globals.initialization_thread = 0;
            widget_globals.processing_inhibited = 0;
            if ( widget_globals.filesystem_check_result == _fscheck_result_not_enough_free_space )
            {
                if ( bink_playback_in_progress() )
                    bink_playback_stop();
                display_error(_error_hard_drive_not_enough_free_space, -1, 1u, 0);
            }
            else if ( widget_globals.filesystem_check_result == _fscheck_result_maximum_saved_games )
            {
                if ( bink_playback_in_progress() )
                    bink_playback_stop();
                display_error(_error_hard_drive_maximum_saved_game_files, -1, 1u, 0);
            }
        }
        return;
    }

    if ( progress_bar_is_active() )
        return;

    if ( virtual_keyboard_active() )
    {
        /* 0x83737124/0x83737128: two bare calls, no argument setup between them. Both are void(void)
         * (virtual_keyboard_process tail-calls a void callee and never defines r3). */
        virtual_keyboard_process();
        event_manager_flush();
        return;
    }

    if ( attract_mode_should_start() )
    {
        attract_mode_start();
        return;
    }

    /* a queued dashboard error takes priority over everything else */
    if ( (uint16_t)widget_globals.deferred_dashboard_error.error_code != 0xFFFF )
    {
        display_error(widget_globals.deferred_dashboard_error.error_code, -1, 1u, 0);
        widget_globals.deferred_dashboard_error.error_code = -1;
        return;
    }

    /* if any generic/cinematic deferred error is pending, surface those instead of
     * pumping the widgets this frame */
    if ( (uint16_t)widget_globals.deferred_error[0].error_code != 0xFFFF
      || (uint16_t)widget_globals.deferred_error[1].error_code != 0xFFFF
      || (uint16_t)widget_globals.deferred_cinematic_error[0].error_code != 0xFFFF )
    {
        _deferred_error *deferred_error;
        for ( deferred_error = widget_globals.deferred_error;
              deferred_error < &widget_globals.deferred_error[2];
              ++deferred_error )
        {
            if ( (uint16_t)deferred_error->error_code != 0xFFFF
              && (we_are_at_the_main_menu || network_game_is_active() || game_time_get() >= 30) )
            {
                display_error(deferred_error->error_code, deferred_error->local_player_index,
                              deferred_error->modal, deferred_error->pause_game_time);
                deferred_error->error_code = -1;
            }
        }
        return;
    }

    /* normal path: dispatch events to each of the two active widget trees */
    {
        unsigned char game_paused = ui_check_for_pause_game();
        unsigned char tree_is_error_dialog[2];
        unsigned char any_error_dialog;
        int tree_index;
        unsigned char consumed;

        tree_is_error_dialog[0] =
            (widget_globals.active_widgets[0]
             && widget_globals.active_widgets[0]->widget_is_error_dialog == 1) ? 1 : 0;
        tree_is_error_dialog[1] =
            (widget_globals.active_widgets[1]
             && widget_globals.active_widgets[1]->widget_is_error_dialog == 1) ? 1 : 0;
        any_error_dialog = tree_is_error_dialog[0] | tree_is_error_dialog[1];

        /* recovered: slot pointer-walk -> active_widgets[2]/widget_stack[2] index loop */
        for ( tree_index = 0; tree_index < 2; ++tree_index )
        {
            widget_instance *widget = widget_globals.active_widgets[tree_index];
            unsigned char process_this;
            ui_widget_definition *definition;
            event_record event[2];

            if ( tree_is_error_dialog[tree_index] == 1 )
                process_this = (widget && widget->widget_is_error_dialog == 1) ? 1 : 0;
            else if ( we_are_at_the_main_menu )
                process_this = (widget && !any_error_dialog) ? 1 : 0;
            else
                process_this = (widget != 0);

            if ( !process_this )
                continue;

            definition = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);
            event[0].type = 0;
            event[0].controller_index = 0;
            event[0].value.value = 0; /* recovered: event[0].value -> event[0].value.value (int union alias) */

            if ( widget_globals.processing_inhibited || !get_next_event(event, widget->local_player_index) )
            {
                if ( !game_paused )
                {
                    event[0].controller_index = widget->local_player_index;
                    widget_instance_process_one_event_recursive(widget, definition, event, &consumed);
                }
            }
            else
            {
                do
                {
                    if ( !game_paused )
                    {
                        widget_instance_process_one_event_recursive(widget, definition, event, &consumed);
                        if ( consumed == 1 )
                            break;
                    }
                }
                while ( widget == widget_globals.active_widgets[tree_index]
                     && get_next_event(event, widget->local_player_index) );
            }

            events_dispatched = 1;

            /* the tree emptied: pop the previous widget off its stack and restore focus */
            if ( !widget_globals.active_widgets[tree_index] )
            {
                _widget_stack_node_data *popped = &widget_globals.widget_stack[tree_index]->data;
                if ( popped )
                {
                    widget_stack_node *node = widget_globals.widget_stack[tree_index];
                    int previous_widget_tag = popped->previous_widget_tag;
                    int focused_parent_tag  = popped->focused_child_parent_widget_tag;
                    /* DEVIATION: decompiler fused the two adjacent int16s into one packed int
                     * (high half = focused_child_index, low half = local_player_index on BE);
                     * untangled into the real members. */
                    int16_t focused_child_index = popped->focused_child_index;
                    int16_t stacked_local_player_index = popped->local_player_index;

                    widget_globals.widget_stack[tree_index] = node->next;
                    dispose_pointer(widget_memory_pool, node);

                    if ( previous_widget_tag != -1 )
                    {
                        widget_instance *restored = ui_widget_load_by_name_or_tag(
                            0, previous_widget_tag, 0, stacked_local_player_index, -1, -1, -1);
                        if ( restored )
                            widget_instance_set_focused_child_by_index(
                                restored, focused_parent_tag, focused_child_index);
                    }
                }
            }
        }

        if ( events_dispatched )
            event_manager_flush(); /* attested void(void): phantom arg removed */
    }
}
