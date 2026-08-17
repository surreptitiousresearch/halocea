/* credits_initialize @0x83697F00 */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"

extern uint32_t system_milliseconds(void);

/* Credits widget setup handler: allocates the 20-byte-stride credit-line array and
 * seeds the scroll timestamp. Returns 1 (event handled). */
uint8_t credits_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    dynamic_array_new(&credits_list, 20);
    credits_last_update_time = system_milliseconds();
    return 1;
}
