/* breakable_surface_damage @0x83743FF8 — subtract damage from a breakable structure surface's vitality
 * and, if it drops to zero, mark the surface destroyed and spawn its break effect.
 *
 * The vitality lives in the per-BSP breakable_surface_globals.breakable_surfaces table. The damage amount
 * is drawn from the damage_effect tag's random vitality-leak range, biased by the damage scale and by a
 * per-material modifier table inside the tag, then normalized by the material's maximum_vitality. When the
 * surface breaks, its destroyed-surface flag bit is cleared and breakable_surface_effect() is fired. */

#include <stdint.h>
#include "headers/breakable_surface_globals.h"
#include "headers/material_definition.h"
#include "headers/damage_data.h"
#include "headers/global_tag_instances.h"
#include "headers/damage_effect_definition.h"

extern material_definition *scenario_material_definition_get(int16_t material_type);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void breakable_surface_effect(int16_t breakable_surface_index, const damage_data *damage_data, int seed_surface_index);

void breakable_surface_damage(int16_t breakable_surface_index, const damage_data *damage_data,
                              int seed_surface_index)
{
    if ( !globals->enabled )
        return;
    if ( breakable_surface_index == -1 || damage_data->definition_index == -1 )
        return;
    if ( (unsigned __int16)damage_data->material_type == 0xFFFF )
        return;

    float *vitality = &globals->breakable_surfaces[global_structure_bsp_index][breakable_surface_index].vitality;
    if ( *vitality <= 0.0f )
        return;

    material_definition *material = scenario_material_definition_get((unsigned __int16)damage_data->material_type);
    if ( !material || material->breakable_surface.maximum_vitality <= 0.0f )
        return;

    /* the damage_effect tag instance that caused the hit */
    damage_effect_definition *damage_effect =
        TAG_GET(damage_effect_definition, damage_data->definition_index);
    float range_maximum = damage_effect->damage.damage_upper_bound;
    float range_minimum = damage_effect->damage.damage_lower_bound;

    float random_leak = real_seed_random_range(get_global_random_seed_address(), range_minimum, range_maximum);
    float vitality_leak_base = damage_effect->damage.damage_minimum;
    /* per-material damage modifier table, indexed by material type */
    float material_modifier = damage_effect->damage.material_modifiers[damage_data->material_type];

    float damage = ((((random_leak - vitality_leak_base) * damage_data->scale) + vitality_leak_base)
                    * material_modifier)
                   / material->breakable_surface.maximum_vitality;

    *vitality -= damage;

    if ( *vitality <= 0.0f )
    {
        /* clear this surface's bit in the per-BSP destroyed-surface bitset */
        unsigned int *flag_dword = (unsigned int *)&globals->breakable_surface_flags
                                        [global_structure_bsp_index][4 * (breakable_surface_index >> 5)];
        *flag_dword &= ~(1u << (breakable_surface_index & 0x1F));
        breakable_surface_effect(breakable_surface_index, damage_data, seed_surface_index);
    }
}
