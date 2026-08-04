/* widget_instance_update_animation_parameters @0x837320F0 — clamps a widget's animation frame indices to
 * non-negative (definition is presumably applying the widget's `definition` frame range, though only the
 * clamp is confirmed by disasm).
 *
 * DEVIATION: the decompiler renders the clamp as `__CFADD__(x, 0x80000000)` carry-flag bit tricks (a
 * "is x negative" test compiled from a plain comparison); simplified here to the equivalent sign check. */

#include "headers/widget_instance.h"

typedef struct ui_widget_definition ui_widget_definition;

void widget_instance_update_animation_parameters(widget_instance *widget, ui_widget_definition *definition)
{
    if (widget->animation_data.first_frame_index < 0)
        widget->animation_data.first_frame_index = 0;

    if (widget->animation_data.last_frame_index < 0)
        widget->animation_data.last_frame_index = 0;
}
