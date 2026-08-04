/* widget_instance_give_focus_directly @ 0x83732150 — make `target` the focused widget. If the
 * target itself cannot receive events, the nearest focusable sibling (searching forward, then
 * the parent's children, then backward) is chosen instead. Clears the old focus chain from the
 * tree root, then sets the focused_child pointer along target's ancestor chain.
 *
 * "focusable" = has event handlers (definition.event_handler_references.count > 0) or is a
 * list/column container (widget_type 2/3), and not flagged never_receive_events. */

#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"

static int widget_is_focusable(widget_instance *w)
{
    ui_widget_definition *def;
    int wt;
    if ( w->never_receive_events )
        return 0;
    def = TAG_GET(ui_widget_definition, w->ui_widget_tag_index);
    if ( def->event_handler_references.count > 0 )
        return 1;
    wt = w->widget_type;
    return wt == _ui_widget_type_spinner_list || wt == _ui_widget_type_column_list;
}

void widget_instance_give_focus_directly(widget_instance *widget, widget_instance *target)
{
    widget_instance *root, *p;
    widget_instance *old_focus;
    widget_instance *candidate;

    root = widget;
    for ( p = widget->parent; p; p = p->parent )
        root = p;
    old_focus = root->focused_child;

    if ( target->never_receive_events == 1 )
    {
        /* search forward from target for a focusable sibling */
        for ( candidate = target->next; candidate; candidate = candidate->next )
        {
            if ( widget_is_focusable(candidate) )
            {
                target = candidate;
                goto apply;
            }
        }

        /* then scan the parent's children list */
        {
            widget_instance *parent = target->parent;
            if ( parent )
            {
                for ( candidate = parent->children; candidate; candidate = candidate->next )
                    if ( widget_is_focusable(candidate) )
                        break;

                if ( candidate == parent->focused_child )
                {
                    /* fall back to searching backward from target */
                    for ( candidate = target->previous; candidate; candidate = candidate->previous )
                        if ( widget_is_focusable(candidate) )
                            break;
                    if ( !candidate )
                        goto apply;
                }
            }
            else
            {
                candidate = 0;
            }
        }

        if ( candidate )
            target = candidate;
    }

apply:
    if ( old_focus )
    {
        if ( target && old_focus->parent == target->parent && old_focus->parent )
        {
            target->parent->focused_child = target;
            return;
        }
        /* clear the existing focus chain */
        do
        {
            old_focus->parent->focused_child = 0;
            old_focus = old_focus->focused_child;
        }
        while ( old_focus );
    }

    for ( ; target->parent; target = target->parent )
        target->parent->focused_child = target;
}
