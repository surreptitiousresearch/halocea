/* widget_instance_expand_bounds_for_spinner @0x837314C0 — for a spinner-list widget whose UI tag has both
 * list header/footer bitmaps SET (list_header_bitmap/list_footer_bitmap indices != -1) and at most one
 * child widget reference, widens the widget's bounds rectangle leftward by 10 pixels (offset by the tag's
 * list_header_bounds.x0 inset) and rightward if the tag's list_footer_bounds.x1 inset would otherwise clip.
 * Tag fields are typed via ui_widget_definition. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_definition.h"
#include "headers/rectangle2d.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"

void widget_instance_expand_bounds_for_spinner(const widget_instance *widget, rectangle2d *bounds)
{
    if ( widget->widget_type != _ui_widget_type_spinner_list )
        return;

    ui_widget_definition *definition =
        TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);

    if ( definition->child_widget_references.count > 1
      || definition->list_footer_bitmap.index == -1
      || definition->list_header_bitmap.index == -1 )
        return;

    int16_t old_x1 = bounds->x1;
    bounds->x0 = definition->list_header_bounds.x0 + bounds->x0 - 10;

    int16_t right_inset = definition->list_footer_bounds.x1;
    if ( right_inset >= old_x1 )
        bounds->x1 = right_inset + 2;
}
