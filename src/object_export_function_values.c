/* object_export_function_values @0x836EFBE8 — recompute an object's four exported "function" scalars (the inputs
 * that drive animated shader/effect functions), one per export slot declared in the object's definition. For each
 * of the 4 slots, the definition names an export source (definition->function_modes[slot]); a source of 0 leaves
 * the slot untouched, otherwise the slot value (object.incoming_function_values[slot]) is set from the object's
 * live state:
 *   1 -> body vitality, 2 -> shield vitality (clamped to <=1), 3 -> current_body_damage, 4 -> current_shield_damage,
 *   5 -> "random once": if the slot already holds 1.0, reseed it from the global random stream,
 *   0x12 -> 1.0 unless object damage_flags bit 2 is set (then 0),
 *   0x13 -> "compass": yaw of the aiming vector relative to scenario north, normalized to [0,1] (kept at the old
 *           value when the aiming vector is near-vertical to avoid gimbal),
 *   default -> a per-object unsigned byte input (object.region_damage[source-10]) scaled by 1/255.
 *
 * DEVIATION: the decompiler invents three trailing double params (FPR-shadow artifacts); the real prototype is
 * (int object_index) only. The atan2 call's arguments were recovered from disasm (0x836EFDA0-836EFDA8):
 * atan2(aiming[1], aiming[2]); the decompiler modeled them as a punned long double pair. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/scenario.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include <math.h>
#include "headers/object_damage_flags.h"
#include "headers/object_export_function_mode.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);


void object_export_function_values(int object_index)
{
    object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    _object_definition *definition = TAG_GET(_object_definition, object->definition_index);

    for ( int slot = 0; slot < 4; slot = (int16_t)(slot + 1) )
    {
        int16_t export_source = definition->function_modes[slot];
        if ( !export_source )
            continue;

        float value = 0.0f;
        switch ( export_source )
        {
            case _object_export_body_vitality:
                value = object->object.body_vitality;
                break;
            case _object_export_shield_vitality:
                value = object->object.shield_vitality;
                if ( value > 1.0f )
                    value = 1.0f;
                break;
            case _object_export_recent_body_damage:
                value = object->object.current_body_damage;
                break;
            case _object_export_recent_shield_damage:
                value = object->object.current_shield_damage;
                break;
            case _object_export_random_constant:
                if ( object->object.incoming_function_values[slot] == 1.0f )
                {
                    unsigned int *seed = get_global_random_seed_address();
                    value = real_seed_random(seed);
                }
                break;
            case _object_export_alive:
                if ( (object->object.damage_flags & (1u << _object_dead_bit)) == 0 )
                    value = 1.0f;
                else
                    value = 0.0f;
                break;
            case _object_export_compass:
            {
                object_datum *current = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
                float *aiming = (float *)((char *)current + current->object.node_matrices.offset);
                if ( __fabs(aiming[3]) >= 0.9950000047683716 )
                {
                    value = object->object.incoming_function_values[slot];
                }
                else
                {
                    float angle = (float)atan2(aiming[1], aiming[2]);
                    float relative = angle - global_scenario->local_north;
                    if ( relative >= PI )
                        relative -= TWO_PI;
                    if ( relative <= -3.1415927f )
                        relative += TWO_PI;
                    value = relative * INV_TWO_PI + 0.5f;
                    if ( value < 0.0f )
                        value = 0.0f;
                    else if ( value > 1.0f )
                        value = 1.0f;
                }
                break;
            }
            default:
                value = object->object.region_damage[(int16_t)(export_source - _object_export_region_00_damage)] * 0.0039215689f;
                break;
        }

        object->object.incoming_function_values[slot] = value;
    }
}
