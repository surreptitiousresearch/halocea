/* widget_instance_get_cumulative_offset @0x83731458 */
#include "headers/widget_globals.h"
#include "headers/point2d.h"

void widget_instance_get_cumulative_offset(const widget_instance *widget, point2d *pOffset)
{
    pOffset->n[0] = 0;
    pOffset->n[1] = 0;

    for ( ; widget; widget = widget->parent )
    {
        pOffset->n[0] += widget->horizontal_offset;
        pOffset->n[1] += widget->vertical_offset;
    }
}
