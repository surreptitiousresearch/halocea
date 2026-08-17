/* devices_initialize @0x837B4A08 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

/* The object_type_definition `initialize` slot is void (*)(void) and no caller consumes r3, so the
   trailing r3 the decompiler rendered as `return result` is the assignment's leftover (B8 wave). */
void devices_initialize(void)
{
    device_groups_data = game_state_data_new("device groups", 1024, 8);
}
