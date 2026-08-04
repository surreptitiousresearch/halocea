#include "headers/widget_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_definition.h"
#include "headers/ui_widget_type.h"

BOOL widget_instance_can_handle_events(widget_instance *widget)
{
    if ( widget->never_receive_events )
        return 0;

    ui_widget_definition *tag_definition = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);
    if ( tag_definition->event_handler_references.count > 0 )
        return 1;

    __int16 widget_type = widget->widget_type;
    return widget_type == _ui_widget_type_spinner_list || widget_type == _ui_widget_type_column_list;
}
