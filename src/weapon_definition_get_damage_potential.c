/* weapon_definition_get_damage_potential @0x8381A360 — estimate a weapon's damage-per-second potential:
 * reads trigger[0]'s final_rate_of_fire (rounds_per_second out-param) and, from its projectile
 * definition, averages the impact_damage and detonation_damage effects' damage bounds and sums them.
 * Fully typed: +560/+544 are the projectile definition's impact_damage/detonation_damage reference
 * indices, +468/+472 the damage_definition bounds, all read through named struct fields below. */

#include "headers/global_tag_instances.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/projectile_definition.h"
#include "headers/damage_effect_definition.h"

float weapon_definition_get_damage_potential(int weapon_definition_index, float *rounds_per_second)
{
    float damage_potential = 0.0f;

    weapon_trigger_definition *trigger = (weapon_trigger_definition *)
        (TAG_GET(weapon_definition, weapon_definition_index))->weapon.triggers.address;

    if ( rounds_per_second )
        *rounds_per_second = trigger->final_rate_of_fire;

    int projectile_index = trigger->projectile.index;
    if ( projectile_index != -1 )
    {
        projectile_definition *projectile = TAG_GET(projectile_definition, projectile_index);

        int impact_damage_index = projectile->projectile.impact_damage.index;
        if ( impact_damage_index != -1 )
        {
            damage_effect_definition *impact =
                TAG_GET(damage_effect_definition, impact_damage_index);
            damage_potential = (impact->damage.damage_upper_bound
                                + impact->damage.damage_lower_bound) * 0.5f;
        }

        int detonation_damage_index = projectile->projectile.detonation_damage.index;
        if ( detonation_damage_index != -1 )
        {
            damage_effect_definition *detonation =
                TAG_GET(damage_effect_definition, detonation_damage_index);
            damage_potential += (detonation->damage.damage_upper_bound
                                 + detonation->damage.damage_lower_bound) * 0.5f;
        }
    }

    return damage_potential;
}
