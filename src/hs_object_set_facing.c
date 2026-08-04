/* hs_object_set_facing @0x837F8280 — orient an object to face a scenario flag's direction without moving it
 * (reposition = false). */

#include <stdint.h>

extern void hs_object_orient(int object_index, int16_t flag_index, uint8_t position, uint8_t facing);

void hs_object_set_facing(int object_index, int16_t flag_index)
{
    hs_object_orient(object_index, flag_index, 0, 1u);
}
