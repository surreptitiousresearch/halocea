/* weapon_set_integrated_light_power @0x836D8E3C — set a weapon object's integrated-light power
 * (e.g. flashlight/scope glow).
 *
 * DEVIATION: the float argument arrives as a double via the soft-float ABI; the stored field is a float. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/blam_data_globals.h"


void weapon_set_integrated_light_power(int weapon_index, float light_power)
{
    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon->weapon.integrated_light_power = light_power;
}
