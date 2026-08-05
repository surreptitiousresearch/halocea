/* actor_customize_unit @0x8371D0E8 — apply an actor-variant definition to a freshly created unit: stamp the
 * variant index, run the HCEX bridge customization, override vitality/turning, randomize change colors (up to
 * 4, stored into the object's primary/secondary color blocks), and grant the variant's default weapon,
 * grenades and equipment. Camouflaged variants (flags 0x10/0x20) get active-camo state set.
 *
 * Deviation: the decompiler reads the variant flags both as a dword (*v3) and reinterpreted as float
 * (LOBYTE(v25)); both are the same flags dword and are read here as an int. The change-color RGB pack
 * (__ROL4__) is R<<16 | G<<8 | B. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/unit_add_weapon_mode.h"
#include "headers/unit_add_equipment_mode.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_placement_data.h"
#include "headers/real_rgb_color.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_variant_definition_flags.h"
#include "headers/actor_variant_change_colors.h"
#include "headers/equipment_definition.h"
#include "headers/equipment_powerup_type.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"

extern void hcex_obj_customize(int id, int tag_index);
extern void hcex_obj_colorize(int id, int color);
extern void object_initialize_vitality(int object_index, const float *custom_body_vitality, const float *custom_shield_vitality);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_delete(int object_index);
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);
extern int16_t unit_add_grenade_type_to_inventory(int unit_index, int16_t grenade_type, int16_t grenade_count);
extern uint8_t unit_add_equipment_to_inventory(int unit_index, int equipment_index, int16_t mode);

void actor_customize_unit(int actor_variant_definition_index, int unit_index)
{
    actor_variant_definition *variant =
        TAG_GET(actor_variant_definition, actor_variant_definition_index);
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_definition *unit_def = TAG_GET(unit_definition, variant->unit_reference.index);

    unit->object.actor_variant_definition = actor_variant_definition_index;
    hcex_obj_customize(unit_index, actor_variant_definition_index);

    if ( variant->unit.maximum_body_vitality > 0.0f || variant->unit.maximum_shield_vitality > 0.0f )
        object_initialize_vitality(unit_index, &variant->unit.maximum_body_vitality,
                                   &variant->unit.maximum_shield_vitality);

    if ( variant->unit.forced_shader_permutation_index )
        unit->object.forced_shader_permutation_index = variant->unit.forced_shader_permutation_index;

    if ( variant->change_colors.count > 0 )
    {
        for ( int16_t color_index = 0; color_index < variant->change_colors.count;
              color_index = (int16_t)(color_index + 1) )
        {
            /* change-color element: 32 bytes, a lower/upper real_rgb_color pair (+ unused) */
            const actor_variant_change_colors *change_color =
                &((const actor_variant_change_colors *)variant->change_colors.address)[color_index];
            if ( color_index < 4 )
            {
                real_rgb_color color;
                float u = real_seed_random(get_global_random_seed_address());
                rgb_colors_interpolate(&color, 1u, &change_color->color_lower_bound,
                                       &change_color->color_upper_bound, u);

                int red   = (uint8_t)(int)(color.n[0] * 255.0f);
                int green = (uint8_t)(int)(color.n[1] * 255.0f);
                int blue  = (uint8_t)(int)(color.n[2] * 255.0f);
                hcex_obj_colorize(unit_index, ((red << 8 | green) << 8) | blue);

                unit->object.base_change_colors[color_index].n[0] = color.n[0];
                unit->object.base_change_colors[color_index].n[1] = color.n[1];
                unit->object.base_change_colors[color_index].n[2] = color.n[2];
                unit->object.outgoing_change_colors[color_index].n[0] = color.n[0];
                unit->object.outgoing_change_colors[color_index].n[1] = color.n[1];
                unit->object.outgoing_change_colors[color_index].n[2] = color.n[2];
            }
        }
    }

    int weapon_index = variant->ranged_combat.reference.index;
    if ( weapon_index != -1 )
    {
        object_placement_data placement;
        object_placement_data_new(&placement, weapon_index, unit_index);
        int weapon_object = object_new(&placement);
        if ( weapon_object != -1 && !unit_add_weapon_to_inventory(unit_index, weapon_object, _unit_add_weapon_as_only_weapon) )
            object_delete(weapon_object);
    }

    if ( (uint16_t)variant->grenade_combat.grenade_type != 0xFFFF )
    {
        int16_t grenade_count = seed_random_range(get_global_random_seed_address(),
                                    variant->items.grenades_lower_bound,
                                    variant->items.grenades_upper_bound + 1);
        unit_add_grenade_type_to_inventory(unit_index, variant->grenade_combat.grenade_type, grenade_count);
    }

    int equipment_index = variant->items.equipment_reference.index;
    if ( equipment_index != -1 )
    {
        int16_t equipment_type =
            TAG_GET(equipment_definition, equipment_index)->equipment.powerup_type;
        if ( equipment_type != _powerup_type_none && equipment_type != _powerup_type_grenade )
        {
            object_placement_data placement;
            object_placement_data_new(&placement, equipment_index, unit_index);
            int equipment_object = object_new(&placement);
            if ( equipment_object != -1 && !unit_add_equipment_to_inventory(unit_index, equipment_object, _unit_add_equipment_as_only_equipment) )
                object_delete(equipment_object);
        }
    }

    int variant_flags = variant->flags;
    if ( (variant_flags & (1u << _actor_variant_definition_active_camouflage_bit)) != 0 || (variant_flags & (1u << _actor_variant_definition_super_active_camouflage_bit)) != 0 )
    {
        if ( (variant_flags & (1u << _actor_variant_definition_super_active_camouflage_bit)) != 0 )
            unit->unit.flags |= (1u << _unit_super_camouflaged_bit);
        unit->unit.flags |= (1u << _unit_active_camouflaged_bit);
        unit->unit.active_camouflage = 1.0f;
        /* dword read of the unit tag's leading {type,flags} pair; bit 0x20 lands in object.flags (BE).
         * LEFT RAW: object_definition_flags (DB $772C7EF8) defines only bits 0-3; bit 5 is genuinely
         * absent from the DB enum, so it cannot be named without fabrication. */
        unit->unit.active_camouflage_super_amount = (unit_def->object.flags & 0x20) == 0 ? 0.0f : 1.0f;
    }
}
