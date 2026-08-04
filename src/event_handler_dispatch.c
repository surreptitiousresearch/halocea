/* event_handler_dispatch @ 0x837348D0 — execute one matched widget event handler. Depending on
 * its flag bits this may: run an hs script command, invoke a handler function, move focus to a
 * named widget, delete a named/own/top-level widget, launch or in-place-replace a widget, go
 * back, tear down all widget stacks, and play a sound effect. If the handler "failed" and has
 * the 0x800 flag, the definition's conditional widgets are launched as a fallback. Reports
 * whether the calling widget was deleted via *calling_widget_deleted. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/event_handler_reference.h"
#include "headers/event_record.h"
#include "headers/conditional_widget_reference.h"
#include "headers/event_handler_flags.h"
#include "headers/conditional_widget_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_audio_feedback_sound.h"

#include "headers/stack_memory_pool.h"
#include "headers/memory_block.h"
#include "headers/stack_memory_pool.h"
extern int hs_evaluate_by_name(const char *script_name);
extern uint8_t ui_widget_event_handler_function_invoke(widget_instance *widget, event_record *event, uint16_t function, unsigned char *widget_deleted);
extern widget_instance *widget_instance_find_by_tag_index_recursive(widget_instance *widget, int tag_index);
extern void widget_instance_give_focus_directly(widget_instance *widget, widget_instance *target);
extern void ui_widget_delete(widget_instance *widget);
extern widget_instance *ui_widget_launch_widget(widget_instance *widget, int new_widget_tag_index);
extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);
extern void widget_instance_go_back_to_previous(widget_instance *widget);
extern int unspatialized_impulse_sound_new(int definition_index, float scale);
extern widget_instance *widget_instance_get_topmost_parent(widget_instance *widget);
extern void dispose_pointer(stack_memory_pool *pool, void *p);
extern int tag_loaded(uint32_t group_tag, const char *name);

void event_handler_dispatch(widget_instance *widget, ui_widget_definition *definition,
                            event_record *event, event_handler_reference *event_handler,
                            unsigned char *calling_widget_deleted)
{
    unsigned char deleted[16];
    char function_failed = 0;
    __int16 sound_code = 0;
    char delete_topmost = 0;
    char self_handled = 0;
    char succeeded = 1;
    char close_all = 0;
    int flags;
    int i;

    deleted[0] = 0;

    if ( (event_handler->flags & (1u << _event_handler_run_scenario_script_bit)) && event_handler->script_command[0] )
        hs_evaluate_by_name(event_handler->script_command);

    if ( (event_handler->flags & (1u << _event_handler_run_function_bit)) == 0
      || ui_widget_event_handler_function_invoke(widget, event, event_handler->function, deleted) )
    {
        unsigned char was_deleted = deleted[0];

        /* 0x40: move focus to a named widget */
        if ( (event_handler->flags & (1u << _event_handler_give_focus_to_widget)) && !deleted[0] )
        {
            int index = event_handler->ui_widget_tag.index;
            if ( index == -1 )
            {
                succeeded = 0;
            }
            else
            {
                widget_instance *root = widget, *p, *target;
                for ( p = widget->parent; p; p = p->parent )
                    root = p;
                target = widget_instance_find_by_tag_index_recursive(root, index);
                if ( target )
                    widget_instance_give_focus_directly(root, target);
                was_deleted = deleted[0];
            }
        }

        flags = event_handler->flags;
        if ( (flags & (1u << _event_handler_reload_widget_bit)) && !was_deleted )
            succeeded = (event_handler->ui_widget_tag.index == -1) ? 0 : succeeded;

        /* 0x1: delete the whole top-level widget afterwards */
        if ( (flags & (1u << _event_handler_close_current_widget_bit)) && !was_deleted )
            delete_topmost = 1;

        /* 0x2: delete a named widget anywhere in the active set */
        if ( (flags & (1u << _event_handler_close_other_widget_bit)) && !was_deleted )
        {
            int index = event_handler->ui_widget_tag.index;
            if ( index != -1 )
            {
                widget_instance *target = 0;
                int slot_index;
                /* recovered: slot pointer-walk -> active_widgets[2] index loop */
                for ( slot_index = 0; slot_index < 2 && !target; ++slot_index )
                {
                    if ( widget_globals.active_widgets[slot_index] )
                        target = widget_instance_find_by_tag_index_recursive(widget_globals.active_widgets[slot_index], index);
                }
                if ( !target )
                {
                    succeeded = 0;
                    goto after_delete_named;
                }
                if ( target == widget )
                    self_handled = 1;
                else
                    ui_widget_delete(target);
            }
        }
    after_delete_named:

        /* 0x4: close everything back to the root */
        if ( (event_handler->flags & (1u << _event_handler_close_all_widgets_bit)) && !deleted[0] )
            close_all = 1;

        /* 0x8: launch a new widget */
        if ( event_handler->flags & (1u << _event_handler_open_widget_bit) )
        {
            int index = event_handler->ui_widget_tag.index;
            if ( index != -1 )
            {
                if ( ui_widget_launch_widget(widget, index) )
                {
                    if ( !sound_code )
                        sound_code = 2;
                    deleted[0] = 1;
                }
                else
                {
                    succeeded = 0;
                }
            }
        }

        /* 0x100: replace this widget in-place with a newly loaded one */
        if ( (event_handler->flags & (1u << _event_handler_replace_with_other_widget_bit)) && !deleted[0] )
        {
            int index = event_handler->ui_widget_tag.index;
            if ( index != -1 )
            {
                widget_instance *replacement =
                    ui_widget_load_by_name_or_tag(0, index, widget, widget->local_player_index, -1, -1, -1);
                if ( replacement )
                {
                    widget_instance *rep_prev = replacement->previous;
                    widget_instance *parent = widget->parent;
                    widget_instance *next = widget->next;
                    widget_instance *prev = widget->previous;
                    int slot_index;

                    if ( rep_prev )
                        rep_prev->next = 0;
                    replacement->previous = 0;
                    replacement->parent = 0;
                    replacement->horizontal_offset = widget->horizontal_offset + replacement->horizontal_offset;
                    replacement->vertical_offset = widget->vertical_offset + replacement->vertical_offset;

                    if ( parent )
                    {
                        replacement->parent = parent;
                        if ( parent->children == widget )
                            parent->children = replacement;
                        if ( parent->focused_child == widget )
                            parent->focused_child = replacement;
                    }
                    if ( next )
                        next->previous = replacement;
                    replacement->next = next;
                    if ( prev )
                        prev->next = replacement;
                    replacement->previous = prev;

                    /* recovered: slot pointer-walk -> active_widgets[2] index loop; clear the matching slot */
                    for ( slot_index = 0; slot_index < 2; ++slot_index )
                    {
                        if ( widget_globals.active_widgets[slot_index] == replacement )
                        {
                            widget_globals.active_widgets[slot_index] = 0;
                            break;
                        }
                    }
                    if ( !sound_code )
                        sound_code = 2;
                    widget->previous = 0;
                    self_handled = 1;
                    widget->next = 0;
                    widget->parent = 0;
                }
                else
                {
                    succeeded = 0;
                }
            }
        }

        /* 0x200: go back to the previous widget */
        if ( event_handler->flags & (1u << _event_handler_go_back_to_previous_widget_bit) )
        {
            widget_instance_go_back_to_previous(widget);
            if ( !sound_code )
                sound_code = 3;
            deleted[0] = 1;
        }

        {
            int snd = event_handler->sound_effect_tag.index;
            if ( snd != -1 )
                unspatialized_impulse_sound_new(snd, 1.0f);
        }

        if ( close_all )
        {
            /* delete every controller's root widget and free its back-stack */
            /* recovered: stack pointer-walk (stack-2 == active_widgets[i]) -> active_widgets[2]/widget_stack[2] index loop */
            for ( i = 0; i < 2; ++i )
            {
                widget_instance *root = widget_globals.active_widgets[i];
                if ( root )
                    ui_widget_delete(root);
                while ( widget_globals.widget_stack[i] )
                {
                    widget_stack_node *node = widget_globals.widget_stack[i];
                    widget_globals.widget_stack[i] = node->next;
                    dispose_pointer(widget_memory_pool, node);
                }
            }
            deleted[0] = 1;
        }
        else if ( delete_topmost )
        {
            ui_widget_delete(widget_instance_get_topmost_parent(widget));
            deleted[0] = 1;
        }
        else if ( self_handled )
        {
            ui_widget_delete(widget);
            deleted[0] = 1;
        }
        else
        {
            goto fallback;   /* no delete action — skip the deleted[0]=1 below */
        }
    }
    else
    {
        function_failed = 1;
        succeeded = 0;
    }

fallback:
    /* 0x800: on failure, launch the definition's conditional widgets */
    if ( !succeeded && (event_handler->flags & (1u << _event_handler_look_for_conditional_widget_on_failure_bit)) && definition->conditional_widget_references.count > 0 )
    {
        for ( i = 0; i < definition->conditional_widget_references.count; ++i )
        {
            const conditional_widget_reference *cond =
                &((const conditional_widget_reference *)definition->conditional_widget_references.address)[i];
            if ( function_failed == 1 && (cond->flags & (1u << _conditional_widget_load_if_event_handler_function_fails_bit)) && !deleted[0] )
            {
                int index = cond->ui_widget_tag.index;
                if ( index != -1 )
                    deleted[0] = (ui_widget_launch_widget(widget, index) != 0);
            }
        }
    }

    /* feedback sound for the action taken — same ui_audio_feedback_sound domain (and tag paths)
     * as ui_play_audio_feedback_sound, inlined */
    if ( sound_code >= _ui_audio_feedback_cursor
      && sound_code <= _ui_audio_feedback_flag_failure )
    {
        const char *sound_name;
        int sound_tag;
        switch ( sound_code )
        {
            case _ui_audio_feedback_forward: sound_name = "sound\\sfx\\ui\\forward"; break;
            case _ui_audio_feedback_back:    sound_name = "sound\\sfx\\ui\\back"; break;
            case _ui_audio_feedback_cursor:  sound_name = "sound\\sfx\\ui\\cursor"; break;
            default: /* _ui_audio_feedback_flag_failure */ sound_name = "sound\\sfx\\ui\\flag_failure"; break;
        }
        sound_tag = tag_loaded(0x736E6421u /* 'snd!' */, sound_name);
        if ( sound_tag != -1 )
            unspatialized_impulse_sound_new(sound_tag, 1.0f);
    }

    *calling_widget_deleted = deleted[0];
}
