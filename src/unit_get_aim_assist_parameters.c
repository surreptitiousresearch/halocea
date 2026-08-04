/* unit_get_aim_assist_parameters @0x836C58B0 — compute the aim-assist angles/distances for a unit's current
 * weapon at a zoom level. Angles scale inversely with zoom magnification, distances scale with it. The weapon
 * tag holds the base values (autoaim angle +0x3E4 / distance +0x3E8, magnetism angle +0x3EC / distance +0x3F0,
 * deviation angle +0x3F4, lower-bounded by the autoaim angle). Weapons whose tag flag bit 0x20 (+0x308) requires
 * zoom give no aim assist when not zoomed. For player units, the three angles are further scaled by the HCEX
 * aim-assist coefficient. Returns 1 on success, 0 if the unit has no weapon (or zoom-gated). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/aim_assist_parameters.h"
#include "headers/weapon_definition.h"   /* for named access to the weapon tag's _weapon_definition level */
#include "headers/weapon_definition_flags.h"
#include "headers/blam_data_globals.h"

extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern float weapon_get_zoom_magnification(int weapon_index, int16_t zoom_level);
extern int player_index_from_unit_index(int unit_index);

uint8_t unit_get_aim_assist_parameters(int unit_index, int16_t zoom_level, aim_assist_parameters *parameters)
{
    if ( unit_index == -1 )
        return 0;

    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int weapon = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);
    if ( weapon == -1 )
        return 0;

    /* recovered: raw int + offset punning of the weapon tag -> typed weapon_definition composite;
     * weapon_tag+776 (0x308) is the embedded _weapon_definition level, +996 (0x3E4) its aim_assist_parameters (+0xDC) */
    weapon_definition *weapon_tag = TAG_GET(weapon_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon)->datum->definition_index);
    if ( zoom_level == -1 && (weapon_tag->weapon.flags & (1u << _weapon_aim_assists_only_when_zoomed_bit)) != 0 )
        return 0;

    float zoom_magnification = weapon_get_zoom_magnification(weapon, zoom_level);
    const aim_assist_parameters *weapon_aim_assist = &weapon_tag->weapon.aim_assist_parameters;
    parameters->autoaim_angle = (1.0f / zoom_magnification) * weapon_aim_assist->autoaim_angle;
    parameters->autoaim_distance = weapon_aim_assist->autoaim_distance * zoom_magnification;
    parameters->magnetism_angle = weapon_aim_assist->magnetism_angle * (1.0f / zoom_magnification);
    parameters->magnetism_distance = weapon_aim_assist->magnetism_distance * zoom_magnification;

    float deviation = weapon_aim_assist->deviation_angle;
    if ( deviation <= weapon_aim_assist->autoaim_angle )
        deviation = weapon_aim_assist->autoaim_angle;
    parameters->deviation_angle = deviation * (1.0f / zoom_magnification);

    if ( player_index_from_unit_index(unit_index) != -1 )
    {
        parameters->autoaim_angle = parameters->autoaim_angle * hcex_aim_assist_coeff;
        parameters->magnetism_angle = parameters->magnetism_angle * hcex_aim_assist_coeff;
        parameters->deviation_angle = parameters->deviation_angle * hcex_aim_assist_coeff;
    }
    return 1;
}
