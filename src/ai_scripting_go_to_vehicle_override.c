/* ai_scripting_go_to_vehicle_override @0x83770F00 — order the addressed actors to enter a vehicle seat
 * (matching the seat-name substring), overriding actors already heading to a different vehicle. */

#include <stdint.h>

extern void ai_scripting_go_to_vehicle_internal(int ai_index, int unit_index, const char *seat_substring_name, uint8_t override_previous_vehicles);

void ai_scripting_go_to_vehicle_override(int ai_index, int unit_index, const char *seat_substring_name)
{
    ai_scripting_go_to_vehicle_internal(ai_index, unit_index, seat_substring_name, 1u);
}
