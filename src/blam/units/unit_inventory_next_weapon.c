/* unit_inventory_next_weapon @0x836D22E0 — pick the next/previous weapon slot in a unit's inventory.
 * Thin wrapper: forwards directly to unit_weapon_next_index. */

#include <stdint.h>

extern int16_t unit_weapon_next_index(int unit_index, int16_t current_index, int16_t delta);

int16_t unit_inventory_next_weapon(int unit_index, int16_t current_index, int16_t delta)
{
    return unit_weapon_next_index(unit_index, current_index, delta);
}
