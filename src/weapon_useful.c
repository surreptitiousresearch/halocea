/* weapon_useful @0x836D9070 — a weapon is still useful while its age has not fully worn out
 * (weapon_datum.weapon.age < 1.0). Used by AI/inventory logic to decide whether to keep or drop it. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/blam_data_globals.h"


BOOL weapon_useful(int weapon_index)
{
    weapon_datum *weapon =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    return weapon->weapon.age < 1.0f;
}
