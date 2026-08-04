/* weapon_prevents_grenade_throwing @0x836D942C — does the unit's current weapon block grenade throws?
 * No weapon (index -1) blocks (returns 1). Otherwise the base answer is the weapon definition flag
 * bit 6, but a weapon mid-action — its weapon state in the range [5,10] — always blocks. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_state.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/blam_data_globals.h"


uint8_t weapon_prevents_grenade_throwing(int weapon_index)
{
    if (weapon_index == -1)
        return 1;

    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    unsigned int weapon_state = (unsigned char)weapon->weapon.state;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    int result = (definition->weapon.flags & _weapon_definition_flag_prevents_grenade_throwing) != 0;

    /* mid-action states [primary_reload .. put_away] always block a grenade throw */
    if (weapon_state >= weapon_state_primary_reload && weapon_state <= weapon_state_put_away)
        return 1;
    return result;
}
