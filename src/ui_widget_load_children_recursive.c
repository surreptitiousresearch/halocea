/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* ui_widget_load_children_recursive @ 0x837332A0 — build a widget's child tree from its
 * definition: for a text-box list, instantiate one child per string-list entry; then load each
 * referenced child widget (with its per-child controller/offsets); load the extended-description
 * widget for list/column containers; and choose an initial focused child. Returns 1 on success,
 * 0 if any child failed to load.
 *
 * child_widget_reference is accessed by raw offset (80-byte stride; flags@48, controller@52,
 * tag index@12, h-offset@56, v-offset@54) — its struct was not reconstructed. */

#include "headers/widget_globals.h"
#include "headers/child_widget_reference.h"
#include "headers/ui_widget_definition.h"
#include "headers/ui_widget_definition_flags.h"
#include "headers/ui_widget_list_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/child_widget_flags.h"
#include "headers/ui_widget_type.h"

extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);

/* append `child` to the end of widget's children list */
static void append_child(widget_instance *widget, widget_instance *child)
{
    widget_instance *cursor = widget->children;
    if ( cursor )
    {
        widget_instance *last = cursor;
        widget_instance **link = &cursor->next;
        while ( cursor )
        {
            link = &cursor->next;
            last = cursor;
            cursor = cursor->next;
        }
        *link = child;
        child->previous = last;
    }
    else
    {
        widget->children = child;
    }
}

int ui_widget_load_children_recursive(widget_instance *widget, ui_widget_definition *widget_definition)
{
    int success = 1;
    int i;
    int widget_type;

    /* text-box list: one child widget per entry in the string list */
    if ( widget_definition->list_flags & (1u << _list_items_generated_from_string_list_tag) )
    {
        int *string_list = TAG_GET(int, widget_definition->text_label_unicode_string_list_tag.index);
        int count = 0;
        widget_globals.dont_load_children_recursive = 1;
        if ( *string_list > 0 )
        {
            while ( 1 )
            {
                widget_instance *item = ui_widget_load_by_name_or_tag(
                    0, widget->ui_widget_tag_index, widget, widget->local_player_index, -1, -1, -1);
                if ( !item )
                {
                    success = 0;
                    break;
                }
                append_child(widget, item);
                ++count;
                ++widget->parameters.list_parameters.number_of_items;
                if ( count >= *string_list )
                    break;
            }
        }
        widget_globals.dont_load_children_recursive = 0;
    }

    /* referenced child widgets */
    for ( i = 0; i < widget_definition->child_widget_references.count; ++i )
    {
        child_widget_reference *ref =
            &((child_widget_reference *)widget_definition->child_widget_references.address)[i];
        __int16 controller = widget->local_player_index;
        int child_tag;
        widget_instance *child;

        if ( (ref->flags & (1u << _child_widget_use_custom_controller_index_bit)) && (unsigned int)ref->custom_controller_index < 4 )
            controller = ref->custom_controller_index;

        child_tag = ref->ui_widget_tag.index;
        if ( child_tag == -1 )
            continue;

        child = ui_widget_load_by_name_or_tag(0, child_tag, widget, controller, -1, -1, -1);
        if ( !child )
        {
            success = 0;
            break;
        }
        child->horizontal_offset = ref->horizontal_offset + widget->horizontal_offset;
        child->vertical_offset = ref->vertical_offset + widget->vertical_offset;
        append_child(widget, child);
    }

    /* extended-description widget for list / column containers */
    widget_type = widget->widget_type;
    if ( widget_type == _ui_widget_type_column_list || widget_type == _ui_widget_type_spinner_list )
    {
        int index = widget_definition->extended_description_widget_tag.index;
        if ( index != -1 )
        {
            widget_instance *desc =
                ui_widget_load_by_name_or_tag(0, index, widget, widget->local_player_index, -1, -1, -1);
            widget->parameters.list_parameters.extended_description = desc;
            if ( desc )
            {
                widget_instance *prev = desc->previous;
                if ( prev )
                    prev->next = 0;
                widget->parameters.list_parameters.extended_description->previous = 0;
                widget->parameters.list_parameters.extended_description->parent = 0;
            }
        }
    }

    /* pick the initial focused child */
    if ( (widget_definition->flags & (1u << _widget_dont_focus_a_specific_child_bit)) == 0 )
    {
        int wt = widget->widget_type;
        char wants_focus = 0;
        if ( wt == _ui_widget_type_spinner_list || wt == _ui_widget_type_column_list )
        {
            widget->parameters.text_box_parameters.string_list_index = 0;
            widget->parameters.list_parameters.last_list_tab_direction = 0;
            widget->parameters.list_parameters.spin_activated = 0;
            wants_focus = 1;
        }
        else if ( widget_definition->flags & (1u << _widget_pass_unhandled_events_to_children_bit) )
        {
            wants_focus = 1;
        }

        if ( wants_focus )
        {
            widget_instance *child = widget->children;
            if ( child )
            {
                while ( wt != _ui_widget_type_spinner_list && wt != _ui_widget_type_column_list )
                {
                    if ( !child->never_receive_events )
                    {
                        ui_widget_definition *child_def = TAG_GET(ui_widget_definition, child->ui_widget_tag_index);
                        int cwt;
                        if ( child_def->event_handler_references.count > 0
                          || (cwt = child->widget_type, cwt == _ui_widget_type_spinner_list)
                          || cwt == _ui_widget_type_column_list )
                            break;
                    }
                    child = child->next;
                    if ( !child )
                        return success;
                }
                widget->focused_child = child;
            }
        }
    }

    return success;
}
