#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void xbox_dashboard_launch(void);

uint8_t exit_to_xbox_dashboard(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    xbox_dashboard_launch();
    return 0;
}
