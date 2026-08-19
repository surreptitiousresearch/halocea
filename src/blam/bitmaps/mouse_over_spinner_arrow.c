/* mouse_over_spinner_arrow @0x836978E8 — decide which half of a spinner widget's (expanded) bounds a point
 * falls in. Fetches the widget's UI-definition bounds (definition +36), expands them for the spinner arrows,
 * then compares the cumulative screen offset against the horizontal midpoint. Returns 1 when it lands on the
 * right (increment) side of the midpoint, otherwise -1.
 *
 * Faithful shipped quirk (same class as widget_instance_is_hot): the "point" tested is the PACKED cumulative
 * offset dword ((y << 16) | x) compared as a 32-bit integer — the compiled code never loads an actual cursor
 * position. Reproduced verbatim. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"

extern void widget_instance_get_cumulative_offset(const widget_instance *widget, point2d *pOffset);
extern void widget_instance_expand_bounds_for_spinner(const widget_instance *widget, rectangle2d *bounds);

int mouse_over_spinner_arrow(widget_instance *widget)
{
    point2d offset;
    widget_instance_get_cumulative_offset(widget, &offset);

    rectangle2d bounds =
        TAG_GET(ui_widget_definition, widget->ui_widget_tag_index)->bounds;
    widget_instance_expand_bounds_for_spinner(widget, &bounds);

    /* n[3] == x1, n[1] == x0; midpoint of the two right/left edges (each offset by the cumulative x). */
    int midpoint_x = ((int16_t)(bounds.n[3] + offset.n[0]) + (int16_t)(bounds.n[1] + offset.n[0])) / 2;
    if ( *(int *)&offset > midpoint_x )
        return 1;
    return -1;
}
