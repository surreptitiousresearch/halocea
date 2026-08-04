/* hs_object_teleport @0x837F8270 — move an object to a scenario flag's location and orientation
 * (reposition = true). */

#include <stdint.h>

extern void hs_object_orient(int object_index, int16_t flag_index, uint8_t position, uint8_t facing);

void hs_object_teleport(int object_index, int16_t flag_index)
{
    hs_object_orient(object_index, flag_index, 1u, 1u);
}
