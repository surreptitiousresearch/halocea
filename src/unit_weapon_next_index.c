/* unit_weapon_next_index @0x836CF768 — pick the weapon slot a unit should switch to. Starting from
 * current_index (treated as 0 when -1), it walks the unit's 4 weapon slots (object+760) in the direction of
 * delta (forward when delta >= 0, backward otherwise). For delta == 0 it selects the usable weapon with the
 * highest "order" value (object+776); for a non-zero delta it stops at the first usable weapon past the
 * current one (and also stops on a weapon that must be readied). Returns the chosen slot, or -1 if none.
 *
 * Weapon object indices live at object+760 (slot*4), the per-slot order values at object+776. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_can_use_weapon(int unit_index, int weapon_index);
extern uint8_t weapon_must_be_readied(int weapon_index);

int16_t unit_weapon_next_index(int unit_index, int16_t current_index, int16_t delta)
{
    __int16 start_slot = current_index == -1 ? 0 : current_index;
    int best_slot = -1;
    int slot = current_index == -1 ? 0 : current_index;
    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    __int16 next_slot;
    do
    {
        /* DEVIATION: raw dword 190+slot is unit.weapon_object_indices[slot]; dword 194+slot is
         * unit.weapon_last_used_at_game_time[slot] (unit@500 + 260/276 + 4*slot) */
        int weapon_index = unit_object->unit.weapon_object_indices[(__int16)slot];
        if ( weapon_index != -1 && unit_can_use_weapon(unit_index, weapon_index) )
        {
            if ( delta
              || (__int16)best_slot == -1
              || unit_object->unit.weapon_last_used_at_game_time[(__int16)best_slot]
                   < unit_object->unit.weapon_last_used_at_game_time[(__int16)slot] )
            {
                best_slot = slot;
            }
            if ( weapon_must_be_readied(unit_object->unit.weapon_object_indices[(__int16)slot])
              || (__int16)slot != start_slot )
                break;
        }

        if ( delta >= 0 )
        {
            next_slot = 0;
            if ( (__int16)slot != MAXIMUM_WEAPONS_PER_UNIT - 1 )
                next_slot = slot + 1;
        }
        else
        {
            next_slot = MAXIMUM_WEAPONS_PER_UNIT - 1;
            if ( (__int16)slot )
                next_slot = slot - 1;
        }
        slot = next_slot;
    }
    while ( next_slot != start_slot );

    return best_slot;
}
