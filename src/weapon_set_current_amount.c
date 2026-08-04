/* weapon_set_current_amount @0x836DA458 — set a weapon object's "fullness" from a 0..1 fraction. For an energy /
 * charge-style weapon (no magazines, or any trigger whose definition charge field is positive) the fraction
 * drives weapon.age (= 1 - fraction). Otherwise it scales the weapon's loaded magazine rounds: the
 * loaded count becomes rounds_loaded_maximum*fraction and rounds_total shifts by the same delta. The
 * charge test is the trigger definition's age_generated_per_round.
 *
 * DEVIATION: the new carried-round value is computed by fctiwz from (float)capacity*fraction; the decompiler
 * modeled the integer store/reload as `(unsigned int)(int)(...) >> 16` with __int64 packing. It is really
 * `(int16_t)(int)((float)capacity * fraction)` — the low word of the truncated product — written directly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/blam_data_globals.h"


void weapon_set_current_amount(int weapon_index, float current_amount)
{
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);

    int magazine_count = definition->weapon.magazines.count;
    uint8_t use_charge = 0;
    if (magazine_count == 0)
    {
        use_charge = 1;
    }
    else
    {
        int trigger_count = definition->weapon.triggers.count;
        if (trigger_count > 0)
        {
            weapon_trigger_definition *triggers =
                (weapon_trigger_definition *)definition->weapon.triggers.address;
            for (int t = 0; t < trigger_count; t = (int16_t)(t + 1))
            {
                if (triggers[t].age_generated_per_round > 0.0f)
                {
                    use_charge = 1;
                    break;
                }
            }
        }
    }

    if (current_amount >= 0.0f)
    {
        if (current_amount > 1.0f)
            current_amount = 1.0f;
    }
    else
    {
        current_amount = 0.0f;
    }

    if (use_charge)
    {
        weapon->weapon.age = 1.0f - current_amount;
    }
    else if (magazine_count > 0)
    {
        weapon_magazine_definition *magazines =
            (weapon_magazine_definition *)definition->weapon.magazines.address;
        int16_t rounds_total = weapon->weapon.magazines[0].rounds_total;
        int16_t rounds_loaded = weapon->weapon.magazines[0].rounds_loaded;
        int16_t magazine_capacity = magazines->rounds_loaded_maximum;
        int16_t new_loaded = (int16_t)(int)((float)magazine_capacity * current_amount);
        weapon->weapon.magazines[0].rounds_loaded = new_loaded;
        weapon->weapon.magazines[0].rounds_total = (rounds_total - rounds_loaded) + new_loaded;
    }
}
