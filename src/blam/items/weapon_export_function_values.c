/* weapon_export_function_values @0x836DAD30 — evaluate the weapon's four "function value" export nodes and
 * publish the results onto the object that actually owns the exported animation channels. It resolves the
 * weapon object, walks up the parent chain (while object flag @0x10 bit 0 is set) to find the root object
 * that receives the values, then for each of the four export function slots (weapon definition word
 * @0x330 + i) computes a 0..1 "charged fraction" from the requested source — trigger charge, magazine
 * rounds, battery/age fractions, overheat, etc. — and stores it at root object float (+0x124 + 4*i).
 *
 * DEVIATION: the decompiler renders the single `int weapon_index` argument as a 64-bit register pun
 * (HIDWORD/WORD1/BYTE3); disasm (0x836DAD60-0x836DADCC) confirms the real arg is weapon_index in r3, the
 * root-climb flag is object flags@0x10 bit 0, and the parent index is @0x11C (+284). Raw offsets throughout
 * (opaque weapon object / definition). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/weapon_magazine_definition.h"
#include "headers/weapon_trigger_definition.h"
#include "headers/object_flags.h"
#include "headers/weapon_trigger_state.h"
#include "headers/weapon_export_function_mode.h"
#include "headers/weapon_datum_flags.h"
#include "headers/blam_data_globals.h"


extern float weapon_trigger_get_charged_fraction(int weapon_index, int16_t trigger_index);
extern int game_time_get(void);
extern uint8_t weapon_reloading(int weapon_index);

#define OBJECT_DATA(idx) \
    ((weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, (idx))->datum)

void weapon_export_function_values(int weapon_index)
{
    weapon_datum *object = OBJECT_DATA(weapon_index);
    weapon_definition *definition = TAG_GET(weapon_definition, object->definition_index);

    /* climb to the root object that receives the exported values */
    weapon_datum *root_object = object;
    if ( object->object.flags & (1u << _object_invisible_bit) )
    {
        while ( 1 )
        {
            if ( root_object->object.parent_object_index == -1 )
                break;
            root_object = OBJECT_DATA(root_object->object.parent_object_index);
            if ( (root_object->object.flags & (1u << _object_invisible_bit)) == 0 )
                break;
        }
    }

    for ( int i = 0; i < 4; i = (int16_t)(i + 1) )
    {
        int16_t function_type = definition->weapon.function_modes[i];
        if ( !function_type )
            continue;

        float charged_fraction = 0.0f;
        switch ( function_type )
        {
            case _weapon_export_heat:
                charged_fraction = object->weapon.heat;
                break;
            case _weapon_export_primary_ammunition:
            case _weapon_export_secondary_ammunition:
            {
                int magazine = (int16_t)(function_type - _weapon_export_primary_ammunition);
                weapon_magazine_definition *magazine_definitions =
                    (weapon_magazine_definition *)definition->weapon.magazines.address;
                if ( magazine < definition->weapon.magazines.count
                  && magazine_definitions[magazine].rounds_loaded_maximum )
                {
                    int16_t capacity = magazine_definitions[magazine].rounds_loaded_maximum;
                    int16_t rounds = object->weapon.magazines[magazine].rounds_loaded;
                    charged_fraction = ((float)rounds / (float)capacity);
                }
                break;
            }
            case _weapon_export_primary_rate_of_fire:
            case _weapon_export_secondary_rate_of_fire:
            {
                int trigger = (int16_t)(function_type - _weapon_export_primary_rate_of_fire);
                if ( trigger < definition->weapon.triggers.count )
                    charged_fraction = object->weapon.triggers[trigger].rate_of_fire;
                break;
            }
            case _weapon_export_ready:
                charged_fraction = 1.0f;
                break;
            case _weapon_export_primary_ejection_port:
            case _weapon_export_secondary_ejection_port:
            {
                int trigger = (int16_t)(function_type - _weapon_export_primary_ejection_port);
                if ( trigger < definition->weapon.triggers.count )
                    charged_fraction = object->weapon.triggers[trigger].ejection_port_position;
                break;
            }
            case _weapon_export_overheated:
                if ( (object->weapon.flags & (1u << _weapon_overheated_bit)) != 0 && definition->weapon.heat_recovery_threshold != 1.0 )
                    charged_fraction = ((object->weapon.heat - definition->weapon.heat_recovery_threshold)
                            / ((float)1.0 - definition->weapon.heat_recovery_threshold));
                break;
            case _weapon_export_primary_charged:
            case _weapon_export_secondary_charged:
            {
                int trigger = (int16_t)(function_type - _weapon_export_primary_charged);
                if ( trigger < definition->weapon.triggers.count )
                    charged_fraction = weapon_trigger_get_charged_fraction(weapon_index, trigger);
                break;
            }
            case _weapon_export_illumination:
            {
                int trigger = 0;
                if ( definition->weapon.triggers.count > 0 )
                {
                    int index = 0;
                    do
                    {
                        weapon_trigger_definition *trigger_def =
                            &((weapon_trigger_definition *)definition->weapon.triggers.address)[index];
                        if ( trigger_def->charging_time > 0.0 )
                        {
                            float fraction = weapon_trigger_get_charged_fraction(weapon_index, trigger);
                            if ( charged_fraction <= (fraction * trigger_def->charged_illumination) )
                                charged_fraction = (fraction * trigger_def->charged_illumination);
                        }
                        if ( object->weapon.triggers[index].state == _weapon_trigger_charged
                          && charged_fraction <= ((((float)1.0 - trigger_def->charged_illumination)
                                        * object->weapon.overcharged) + trigger_def->charged_illumination) )
                        {
                            charged_fraction = ((((float)1.0 - trigger_def->charged_illumination)
                                        * object->weapon.overcharged) + trigger_def->charged_illumination);
                        }
                        if ( charged_fraction <= object->weapon.triggers[index].illumination )
                            charged_fraction = object->weapon.triggers[index].illumination;
                        object->weapon.triggers[index].illumination = charged_fraction;
                        trigger = (int16_t)(index + 1);
                        index = trigger;
                    }
                    while ( trigger < definition->weapon.triggers.count );
                }
                if ( charged_fraction <= (definition->weapon.heat_illumination * object->weapon.heat) )
                    charged_fraction = (definition->weapon.heat_illumination * object->weapon.heat);
                break;
            }
            case _weapon_export_age:
                charged_fraction = object->weapon.age;
                break;
            case _weapon_export_integrated_light:
                charged_fraction = object->weapon.integrated_light_power;
                break;
            case _weapon_export_primary_firing:
            case _weapon_export_secondary_firing:
            {
                int trigger = (int16_t)(function_type - _weapon_export_primary_firing);
                if ( trigger < definition->weapon.triggers.count )
                {
                    charged_fraction = object->weapon.triggers[trigger].rate_of_fire;
                    if ( game_time_get() - object->weapon.game_time_last_fired > 1 )
                        charged_fraction = 0.0f;
                }
                break;
            }
            case _weapon_export_primary_firing_on:
            case _weapon_export_secondary_firing_on:
            {
                int trigger = (int16_t)(function_type - _weapon_export_primary_firing_on);
                if ( trigger < definition->weapon.triggers.count )
                {
                    charged_fraction = object->weapon.triggers[trigger].rate_of_fire;
                    if ( !object->weapon.magazines[trigger].rounds_loaded
                      || (object->weapon.flags & (1u << _weapon_overheated_bit)) != 0
                      || weapon_reloading(weapon_index) )
                        charged_fraction = 0.0f;
                }
                break;
            }
            default:
                break;
        }
        root_object->object.incoming_function_values[i] = charged_fraction;
    }
}
