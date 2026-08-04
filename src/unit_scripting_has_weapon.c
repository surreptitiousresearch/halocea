/* unit_scripting_has_weapon @0x83800BC8 — return whether a unit currently carries a weapon of the given
 * weapon definition. */

#include <stdint.h>

extern uint8_t unit_has_weapon_definition_index(int unit_index, int weapon_definition_index);

uint8_t unit_scripting_has_weapon(int unit_index, int weapon_definition_index)
{
    if ( unit_index != -1 && weapon_definition_index != -1 )
        return unit_has_weapon_definition_index(unit_index, weapon_definition_index);
    return 0;
}
