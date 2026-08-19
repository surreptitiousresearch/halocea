#include <stdint.h>
/* hs_trigger_volume_test_objects_all @0x837F8980 — return true only if every object in the list is inside the
 * trigger volume. */

extern int hs_trigger_volume_test_objects(int16_t trigger_volume_index, int object_list_index, int and);

uint8_t hs_trigger_volume_test_objects_all(int16_t trigger_volume_index, int object_list_index)
{
    return hs_trigger_volume_test_objects(trigger_volume_index, object_list_index, 1u);
}
