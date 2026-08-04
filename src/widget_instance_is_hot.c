/* widget_instance_is_hot @0x83731540 — hit-test a widget: fetch its definition bounds (definition +36),
 * expand for spinners, and test against its cumulative screen offset.
 *
 * Faithful shipped quirk (bug class 12, disasm-verified): the "point" tested is the PACKED cumulative
 * offset dword ((y << 16) | x) compared as a 32-bit integer against the sign-extended bound+offset sums —
 * the compiled code never loads a cursor position. Reproduced verbatim. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"

extern void widget_instance_get_cumulative_offset(const widget_instance *widget, point2d *pOffset);
extern void widget_instance_expand_bounds_for_spinner(const widget_instance *widget, rectangle2d *bounds);

uint8_t widget_instance_is_hot(widget_instance *widget)
{
    rectangle2d bounds =
        TAG_GET(ui_widget_definition, widget->ui_widget_tag_index)->bounds;
    point2d offset;
    widget_instance_get_cumulative_offset(widget, &offset);
    widget_instance_expand_bounds_for_spinner(widget, &bounds);

    if ( *(int *)&offset < (__int16)(bounds.n[1] + offset.n[0]) )
        return 0;
    if ( *(int *)&offset > (__int16)(bounds.n[3] + offset.n[0]) )
        return 0;
    if ( *(int *)&offset < (__int16)(bounds.n[0] + offset.n[1]) )
        return 0;
    if ( *(int *)&offset > (__int16)(bounds.n[2] + offset.n[1]) )
        return 0;
    return 1;
}
