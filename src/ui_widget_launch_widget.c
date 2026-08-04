/* ui_widget_launch_widget @ 0x83736EE8 — open a new widget as the successor of `widget`,
 * recording the current widget tree (root tag, parent tag, and the widget's index among its
 * siblings) so it can be restored on back-navigation. The new widget's controller is taken
 * from its own player-count when it requests one (flag 0x1000), else inherited. */

#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/widget_event_flags.h"

extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);

widget_instance *ui_widget_launch_widget(widget_instance *widget, int new_widget_tag_index)
{
    ui_widget_definition *def = TAG_GET(ui_widget_definition, new_widget_tag_index);
    unsigned int player_count = def->controller_index;   /* field at +2 reused as player count */
    __int16 controller;
    widget_instance *root, *p;
    int parent_tag;
    __int16 sibling_index = -1;
    widget_instance *parent = widget->parent;

    /* pick the controller index from the player-count when the widget asks for it */
    if ( (def->flags & (1u << _widget_always_use_tag_controller_index_bit)) )
    {
        if ( player_count > 4 )
            controller = widget->local_player_index;
        else if ( player_count == 1 ) controller = 1;
        else if ( player_count == 2 ) controller = 2;
        else if ( player_count == 3 ) controller = 3;
        else if ( player_count == 0 ) controller = 0;
        else                          controller = -1;
    }
    else
    {
        if ( player_count > 4 )
            controller = widget->local_player_index;   /* v14 — caller's prior value */
        else if ( player_count == 1 ) controller = 1;
        else if ( player_count == 2 ) controller = 2;
        else if ( player_count == 3 ) controller = 3;
        else if ( player_count == 0 ) controller = 0;
        else                          controller = widget->local_player_index;
    }

    /* topmost ancestor + its tag, and the widget's index within its parent's children */
    root = widget;
    if ( parent )
    {
        for ( p = widget->parent; p; p = p->parent )
            root = p;
        parent_tag = parent->ui_widget_tag_index;

        {
            widget_instance *child = parent->children;
            __int16 index = 0;
            if ( child )
            {
                while ( child != widget )
                {
                    child = child->next;
                    ++index;
                    if ( !child )
                        return ui_widget_load_by_name_or_tag(0, new_widget_tag_index, 0, controller,
                                                             root->ui_widget_tag_index, parent_tag, sibling_index);
                }
                sibling_index = index;
            }
        }
    }
    else
    {
        parent_tag = -1;
    }

    return ui_widget_load_by_name_or_tag(0, new_widget_tag_index, 0, controller,
                                         root->ui_widget_tag_index, parent_tag, sibling_index);
}
