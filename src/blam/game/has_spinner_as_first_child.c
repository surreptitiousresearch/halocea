/* has_spinner_as_first_child @0x83780A90 */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_type.h"

uint8_t has_spinner_as_first_child(widget_instance *widget)
{
    widget_instance *first_child = widget->children;
    if ( !first_child )
        return 0;
    widget_instance *grandchild = first_child->children;
    if ( !grandchild )
        return 0;
    return grandchild->widget_type == _ui_widget_type_spinner_list;
}
