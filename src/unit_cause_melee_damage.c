/* unit_cause_melee_damage @0x836D0070 — apply a unit's melee strike. Resolves the "melee" marker on the unit,
 * line-of-sight tests from the unit's origin to the marker (if blocked, the strike originates at the unit
 * itself), selects the damage effect (the unit's default melee effect, or the equipped weapon's melee override
 * if that weapon flags it), builds a damage_data event, and routes it either to a specific target object or to
 * an area-of-effect blast. Finally it plays the melee impact sound (unless silent) and clears the unit's
 * melee-in-progress flag.
 *
 * DEVIATION: the decompiler aliased the damage_data buffer over an object_marker stack slot and mis-merged two
 * adjacent stores (location vs owner_team_index). Field assignments below were re-derived from the store
 * instructions (damage_data layout from the database). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/collision_test_flags.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/unit_definition.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_definition_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/damage_data.h"
#include "headers/object_marker.h"
#include "headers/collision_result.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/unit_melee_attack_state.h"
#include "headers/blam_data_globals.h"


extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void area_of_effect_cause_damage(damage_data *damage_data, int unlucky_object_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern void unit_melee_sound(int unit_index, int damage_definition_index, int16_t material_type);

void unit_cause_melee_damage(int unit_index, uint8_t silent, int target_object_index,
                             int16_t target_node_index, int16_t target_region_index,
                             int16_t target_material_index, const real_vector3d *target_object_normal)
{
    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *definition_tag = TAG_GET(unit_definition, unit_data->definition_index);

    if ( definition_tag->unit.melee_damage.index != -1 )
    {
        real_point3d *origin = &unit_data->object.bounding_sphere_center;
        float impact_x, impact_y, impact_z;

        object_marker melee_marker;
        if ( object_get_marker_by_name(unit_index, "melee", &melee_marker, 1) == 1 )
        {
            real_vector3d to_marker;
            to_marker.n[0] = melee_marker.matrix.n[3][0] - unit_data->object.bounding_sphere_center.n[0];
            to_marker.n[1] = melee_marker.matrix.n[3][1] - unit_data->object.bounding_sphere_center.n[1];
            to_marker.n[2] = melee_marker.matrix.n[3][2] - unit_data->object.bounding_sphere_center.n[2];

            impact_x = melee_marker.matrix.n[3][0];
            impact_y = melee_marker.matrix.n[3][1];
            impact_z = melee_marker.matrix.n[3][2];

            collision_result collision;
            if ( collision_test_vector(_collision_test_for_projectiles_flags /* 0x1000E9 */, origin, &to_marker, -1, &collision) )
            {
                impact_x = unit_data->object.bounding_sphere_center.n[0];
                impact_y = unit_data->object.bounding_sphere_center.n[1];
                impact_z = unit_data->object.bounding_sphere_center.n[2];
            }
        }
        else
        {
            impact_x = unit_data->object.bounding_sphere_center.n[0];
            impact_y = unit_data->object.bounding_sphere_center.n[1];
            impact_z = unit_data->object.bounding_sphere_center.n[2];
        }

        int damage_definition_index = definition_tag->unit.melee_damage.index;

        /* if a weapon is equipped that flags its own melee damage, use that instead */
        __int16 weapon_slot = unit_data->unit.current_weapon_index;
        if ( weapon_slot != -1 )
        {
            int weapon_object_index = unit_data->unit.weapon_object_indices[weapon_slot];
            if ( weapon_object_index != -1 )
            {
                weapon_datum *weapon_data = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data,
                    object_header_datum, weapon_object_index)->datum;
                weapon_definition *weapon_definition_tag =
                    TAG_GET(weapon_definition, weapon_data->definition_index);
                if ( (weapon_definition_tag->weapon.flags & (1u << _weapon_non_players_use_melee_damage_bit)) != 0 )
                    damage_definition_index = weapon_definition_tag->weapon.melee_attack_damage.index;
            }
        }

        damage_data damage;
        damage_data_new(&damage, damage_definition_index);
        damage.owner_player_index = unit_data->unit.player_index;
        damage.owner_object_index = unit_index;
        damage.owner_team_index = unit_data->object.owner_team_index;
        damage.location = unit_data->object.location;
        damage.origin.n[0] = impact_x;
        damage.origin.n[1] = impact_y;
        damage.origin.n[2] = impact_z;
        damage.epicenter.n[0] = unit_data->object.bounding_sphere_center.n[0];
        damage.epicenter.n[1] = unit_data->object.bounding_sphere_center.n[1];
        damage.epicenter.n[2] = unit_data->object.bounding_sphere_center.n[2];

        if ( target_object_index == -1 )
            area_of_effect_cause_damage(&damage, -1);
        else
            object_cause_damage(&damage, target_object_index, target_node_index, target_region_index,
                                target_material_index, target_object_normal);

        if ( !silent && damage.material_type != (__int16)0xFFFF )
            unit_melee_sound(unit_index, damage_definition_index, damage.material_type);
    }

    unit_data->unit.melee_attack_state = _unit_melee_attack_none;
}
