/* weapon_prevents_melee_attack @0x836DB16C — true if a weapon blocks melee while readied: a missing
 * weapon (-1) counts as preventing; otherwise the result is the weapon definition's "prevents melee"
 * flag (bit 9 of the weapon definition flags), overridden to true when the weapon's primary trigger
 * state is charging or charged. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/weapon_trigger_state.h"
#include "headers/blam_data_globals.h"


uint8_t weapon_prevents_melee_attack(int weapon_index)
{
    if ( weapon_index == -1 )
        return 1;

    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    char trigger_state = weapon->weapon.triggers[0].state;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    int prevents = (definition->weapon.flags & _weapon_definition_flag_prevents_melee_attack) != 0;

    if ( trigger_state == _weapon_trigger_charging || trigger_state == _weapon_trigger_charged )
        return 1;
    return prevents;
}
