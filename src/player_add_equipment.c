/* player_add_equipment @0x836AAAD8 — give a unit the weapons, ammo, health/shield and grenades from a
 * scenario player-starting-profile. When `reset` is set, existing weapons and the accumulator fields are
 * cleared first. The profile's primary (entry 2) and secondary (entry 3) weapons are created and added to
 * the inventory (deleting the object if it can't be carried); the profile's starting health/shield
 * floats and its two grenade-type counts are added to the unit. */

#include <stdint.h>
#include "headers/unit_add_weapon_mode.h"
#include "headers/scenario.h"
#include "headers/starting_weapon_info.h"
#include "headers/scenario_starting_profile.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_delete_all_weapons(int unit_index);
extern int create_weapon(const starting_weapon_info *weapon_info, int unit_index);
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);
extern void object_delete(int object_index);

void player_add_equipment(int unit_index, int16_t profile_index, uint8_t reset)
{
    if (unit_index == -1 || profile_index == -1)
        return;

    unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if (unit->unit.player_index == -1)
        return;

    /* DEVIATION: decompiler viewed the 104-byte record as a run of 20-byte starting_weapon_info
     * slots with float puns; rewritten against the DB scenario_starting_profile layout (all offsets
     * verified: [2]=0x28 weapons[0], [3]=0x3C weapons[1], puns = starting_health/shield @0x20/0x24,
     * grenade bytes @0x50). */
    const scenario_starting_profile *profile =
        &((const scenario_starting_profile *)global_scenario->starting_profiles.address)[profile_index];

    if (reset)
    {
        unit_delete_all_weapons(unit_index);
        unit->object.shield_vitality = 0.0f;
        unit->object.body_vitality = 0.0f;
        for (int i = 0; i < 2; i = (int16_t)(i + 1))
            unit->unit.grenade_counts[i] = 0;
    }

    if (profile->starting_weapons[0].weapon.index != -1)
    {
        int weapon = create_weapon(&profile->starting_weapons[0], unit_index);
        if (weapon != -1 && !unit_add_weapon_to_inventory(unit_index, weapon,
                reset ? _unit_add_weapon_as_current_weapon : _unit_add_weapon_normally))
            object_delete(weapon);
    }
    if (profile->starting_weapons[1].weapon.index != -1)
    {
        int weapon = create_weapon(&profile->starting_weapons[1], unit_index);
        if (weapon != -1 && !unit_add_weapon_to_inventory(unit_index, weapon, _unit_add_weapon_normally))
            object_delete(weapon);
    }

    unit->object.shield_vitality += profile->starting_shield;
    unit->object.body_vitality += profile->starting_health;
    for (int i = 0; i < 2; i = (int16_t)(i + 1))
        unit->unit.grenade_counts[i] += profile->starting_grenade_counts[i];
}
