/* unit_export_function_values @0x836C6D10 — evaluate a unit's up-to-4 "export to function" outputs and store
 * them into the object's exported-value slots (object+292 + 4*i) for downstream animation/shader functions.
 * The unit tag definition lists an export-source type per slot (words at definition+408 + 2*i); type 0 leaves
 * the slot untouched, types 1-6 read a live object scalar (body vitality, shield, integrated light power,
 * etc.), type 7 computes a normalized animation frame position, and any other type yields 0.
 *
 * DEVIATION: Hex-Rays reported "local variable allocation has failed" and rendered the type-7 case as a
 * division by an unresolved `*(__int64*)(&v1 - 1)` (an r2-overlapped register pun). Disasm
 * (0x836C6E24-0x836C6EA8) shows the real computation: with marker_index = object+208 and the animation record
 * animation = (*(char**)TAG_INSTANCE(object+204))[0x78 block address] + 180*marker_index, and limit =
 * animation+46, the value is (float)marker_index / (float)limit when marker_index < limit, else
 * 1 - object[526]/90. Reconstructed accordingly.
 *
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"
#include "headers/global_tag_instances.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/unit_export_function_mode.h"
#include "headers/blam_data_globals.h"


void unit_export_function_values(int unit_index)
{
    unit_datum *object =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_definition *definition = TAG_GET(unit_definition, object->definition_index);

    for ( int slot = 0; slot < 4; slot = (int16_t)(slot + 1) )
    {
        int16_t export_type = definition->unit.function_modes[slot];
        if ( !export_type )
            continue;

        float value = 0.0f;
        if ( export_type >= _unit_export_driver_seat_power && export_type <= _unit_export_shield_sapping )
        {
            switch ( export_type )
            {
                case _unit_export_driver_seat_power:
                    value = object->unit.seat_power[0];
                    break;
                case _unit_export_gunner_seat_power:
                    value = object->unit.seat_power[1];
                    break;
                case _unit_export_aiming_change:
                    value = object->unit.aiming_change * 0.0039215689f; /* /255 */
                    break;
                case _unit_export_mouth_aperture:
                    value = object->unit.mouth_aperture;
                    break;
                case _unit_export_integrated_light_power:
                    value = object->unit.integrated_light_power;
                    break;
                case _unit_export_can_blink:
                    value = ((object->object.damage_flags & (1u << _object_dead_bit)) != 0
                             || (object->unit.flags & (1u << _unit_cannot_blink_bit)) != 0) ? 0.0f : 1.0f;
                    break;
                default: /* _unit_export_shield_sapping (7): normalized animation frame position */
                {
                    int16_t marker_index = object->object.animation.state.index;
                    animation_graph *graph =
                        TAG_GET(animation_graph, object->object.animation.animation_graph_index);
                    animation *anim = &((animation *)graph->animations.address)[marker_index];
                    int16_t frame_limit = anim->private_loop_frame_index;
                    if ( marker_index < frame_limit )
                        value = (float)marker_index / (float)frame_limit;
                    else
                        value = 1.0f - (float)object->unit.shield_sap_timeout * 0.011111111f; /* 1 - x/90 */
                    break;
                }
            }
        }

        object->object.incoming_function_values[slot] = value;
    }
}
