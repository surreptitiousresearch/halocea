/* run_xdemos @0x83697338 */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void main_run_demos(void);

uint8_t run_xdemos(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    main_run_demos();
    return 1;
}
