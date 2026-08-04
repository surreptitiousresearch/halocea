/* widget_instance_render_column_list @0x83735320 — render a list-type widget's extended-description child
 * (inheriting the full ancestor-chain alpha) and, if the definition flags a single-column list, each visible
 * child item up to number_of_items, marking the currently-selected one via string_list_index. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_definition.h"
#include "headers/widget_event_flags.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"

extern void widget_instance_render_recursive(widget_instance *widget, rectangle2d *clip_rect, point2d offset, uint8_t focus, uint8_t use_nifty_plasma_fx);

void widget_instance_render_column_list(widget_instance *widget, ui_widget_definition *definition,
                                         rectangle2d *clip_rect, point2d offset, uint8_t focus)
{
    widget_instance *extended_description = widget->parameters.list_parameters.extended_description;
    if ( extended_description )
    {
        float alpha = widget->alpha_modifier;
        for ( widget_instance *parent = widget->parent; parent; parent = parent->parent )
            alpha = parent->alpha_modifier * alpha;
        extended_description->alpha_modifier = alpha;

        {
            unsigned __int8 focus = 0;
            unsigned __int8 use_nifty_plasma_fx = 1;
            widget_instance_render_recursive(extended_description, clip_rect, offset, focus, use_nifty_plasma_fx);
        }
    }

    if ( (definition->list_flags & (1u << _list_items_generated_in_code)) != 0 )
    {
        __int16 item_index = 0;
        for ( widget_instance *child = widget->children; child; child = child->next )
        {
            if ( item_index >= (__int16)widget->parameters.list_parameters.number_of_items )
                break;

            unsigned __int8 selected = widget->parameters.text_box_parameters.string_list_index == item_index;
            item_index++;
            widget_instance_render_recursive(child, clip_rect, offset, focus, selected);
        }
        widget->parameters.list_parameters.last_list_tab_direction = 0;
    }
    else
    {
        widget->parameters.list_parameters.last_list_tab_direction = 0;
    }
}
