/* widget_instance_get_cumulative_alpha_modifier @0x83731620 — a widget's own alpha_modifier multiplied by
 * every ancestor's alpha_modifier up the parent chain. */

#include "headers/widget_instance.h"

float widget_instance_get_cumulative_alpha_modifier(widget_instance *widget)
{
    float alpha = widget->alpha_modifier;
    for (widget_instance *parent = widget->parent; parent; parent = parent->parent)
        alpha *= parent->alpha_modifier;
    return alpha;
}
