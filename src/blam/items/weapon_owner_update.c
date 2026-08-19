/* weapon_owner_update @0x836D8C38 — push the owner's control state into a weapon object: store the
 * control flags and the eased primary-trigger value (via transition function 4).
 *
 * DEVIATION: primary_trigger arrives as a double via the soft-float ABI; it is a float. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/transition_function.h"
#include "headers/blam_data_globals.h"

extern float transition_function_evaluate(int16_t function_type, float value);

/* control_flags is uint16_t: sth into the u16 weapon.control_flags field @0x230 (disasm 836D8C70) */
void weapon_owner_update(int weapon_index, uint16_t control_flags, float primary_trigger)
{
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon->weapon.control_flags = control_flags;
    weapon->weapon.primary_trigger = transition_function_evaluate(_transition_function_very_late, primary_trigger);
}
