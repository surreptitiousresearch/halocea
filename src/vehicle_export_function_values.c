/* vehicle_export_function_values @0x83760620 — recompute a vehicle's four exported "function" scalars (the inputs
 * that drive animated shader/effect functions), one per export slot declared in the vehicle definition. The
 * vehicle analogue of object_export_function_values, with a much larger source vocabulary covering the vehicle's
 * live control/physics state. For each of the 4 slots the definition names an export source
 * (definition_u16[i + 398], i.e. definition+796+2*i); a source of 0 leaves the slot untouched, otherwise the slot
 * value (vehicle[+292 + 4*slot]) is set from the vehicle's live state and (for most sources) clamped to [0,1].
 *
 * The three definition range constants are precomputed once: the forward/reverse range (max |def[190]|,|def[191]|),
 * the slide range (max |def[204]|,|def[205]|), and the turn range (max |def[194]|,|def[195]|); the individual
 * half-ranges are also used directly by several sources. def[196] is the divisor for the auxiliary-axis sources.
 *
 * DEVIATIONS: the source-0x23 blend byte is a plain (float)(unsigned byte) — the decompiler's HIDWORD(v18)=
 * 0x82000000 magic is the fcfid int->float helper, not a real 64-bit value (disasm 0x83760AD8-0x83760AF8:
 * lbz/std/lfd/fcfid/frsp). The many double-typed locals are float-widening artifacts; the real prototype is
 * (int vehicle_index) only. component_vectors_from_normal3d's 4 args were confirmed from disasm
 * (0x83760A88-0x83760A98): (velocity, forward, parallel_out, perpendicular_out). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/object_flags.h"
#include "headers/vehicle_export_function_mode.h"
#include "headers/vehicle_flags.h"
#include "headers/blam_data_globals.h"


extern float __fabs(float x);
extern float __fsqrts(float x);
extern void component_vectors_from_normal3d(const real_vector3d *vector, const real_vector3d *normal, real_vector3d *parallel, real_vector3d *perpendicular);


void vehicle_export_function_values(int vehicle_index)
{
    vehicle_datum *vehicle = (vehicle_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_definition *definition = TAG_GET(vehicle_definition, vehicle->definition_index);

    float forward_scale = __fabs(definition->vehicle.speed.maximum_forward_speed);
    float reverse_scale = __fabs(definition->vehicle.speed.maximum_reverse_speed);
    float forward_reverse_range = reverse_scale >= forward_scale ? reverse_scale : forward_scale;

    float slide_scale_a = __fabs(definition->vehicle.slide.maximum_forward_speed);
    float slide_scale_b = __fabs(definition->vehicle.slide.maximum_reverse_speed);
    float slide_range = slide_scale_a <= slide_scale_b ? slide_scale_b : slide_scale_a;

    float turn_scale_a = __fabs(definition->vehicle.turn.maximum_position);
    float turn_scale_b = __fabs(definition->vehicle.turn.minimum_position);
    float turn_range = turn_scale_a <= turn_scale_b ? turn_scale_b : turn_scale_a;

    for ( int i = 0; i < 4; i = (int16_t)(i + 1) )
    {
        int16_t export_source = definition->vehicle.function_modes[i];
        if ( !export_source )
            continue;

        float throttle = vehicle->vehicle.speed;
        float slide = vehicle->vehicle.slide;
        float turn = vehicle->vehicle.turn;
        const float *velocity = vehicle->object.translational_velocity.n;
        const float *forward = vehicle->object.forward.n;
        const float *up = vehicle->object.up.n;
        float value = 0.0f;

        switch ( export_source )
        {
            case _vehicle_export_speed_absolute:
            case _vehicle_export_front_left_tire_velocity:
            case _vehicle_export_front_right_tire_velocity:
            case _vehicle_export_back_left_tire_velocity:
            case _vehicle_export_back_right_tire_velocity:
                value = __fabs(throttle) / forward_reverse_range;
                goto clamp_and_write;
            case _vehicle_export_speed_forward:
                value = (throttle >= 0.0f ? throttle : 0.0f) / forward_scale;
                goto clamp_and_write;
            case _vehicle_export_speed_backward:
            {
                float reverse = throttle;
                if ( reverse > 0.0f )
                    reverse = 0.0f;
                value = __fabs(reverse) / reverse_scale;
                goto clamp_and_write;
            }
            case _vehicle_export_slide_absolute:
                value = __fabs(slide) / slide_range;
                goto clamp_and_write;
            case _vehicle_export_slide_left:
                value = __fabs(slide) / slide_scale_a;
                goto clamp_and_write;
            case _vehicle_export_slide_right:
                value = __fabs(slide) / slide_scale_b;
                goto clamp_and_write;
            case _vehicle_export_speed_slide_maximum:
            {
                float throttle_ratio = __fabs(throttle) / forward_reverse_range;
                float slide_ratio = __fabs(slide) / slide_range;
                value = throttle_ratio <= slide_ratio ? slide_ratio : throttle_ratio;
                goto clamp_and_write;
            }
            case _vehicle_export_turn_absolute:
                value = __fabs(turn) / turn_range;
                goto clamp_and_write;
            case _vehicle_export_turn_left:
                value = __fabs(turn) / turn_scale_a;
                goto clamp_and_write;
            case _vehicle_export_turn_right:
                value = __fabs(turn) / turn_scale_b;
                goto clamp_and_write;
            case _vehicle_export_crouch:
                value = (vehicle->vehicle.flags & (1u << _vehicle_control_crouch_bit)) != 0 ? 1.0f : 0.0f;
                goto write_value;
            case _vehicle_export_jump:
                value = (vehicle->vehicle.flags & (1u << _vehicle_control_jump_bit)) != 0 ? 1.0f : 0.0f;
                goto write_value;
            case _vehicle_export_velocity_air:
                value = __fsqrts(((velocity[2] * velocity[2])
                                + ((velocity[0] * velocity[0]) + (velocity[1] * velocity[1]))))
                        / forward_reverse_range;
                goto clamp_and_write;
            case _vehicle_export_velocity_water:
                /* 0x1C = the three media-submersion bits (in water/media) */
                if ( (vehicle->object.flags & ((1u << _object_on_media_bit)
                        | (1u << _object_partially_under_media_bit)
                        | (1u << _object_wholly_under_media_bit))) != 0 )
                    value = __fsqrts(((velocity[0] * velocity[0])
                                    + ((velocity[1] * velocity[1]) + (velocity[2] * velocity[2]))))
                            / forward_reverse_range;
                goto clamp_and_write;
            case _vehicle_export_velocity_ground:
                if ( (vehicle->object.flags & (1u << _object_on_ground_bit)) != 0 )
                    value = __fsqrts(((velocity[0] * velocity[0])
                                    + ((velocity[1] * velocity[1]) + (velocity[2] * velocity[2]))))
                            / forward_reverse_range;
                goto clamp_and_write;
            case _vehicle_export_velocity_forward:
                value = __fabs(((velocity[0] * forward[0])
                                + ((forward[2] * velocity[2]) + (forward[1] * velocity[1]))))
                        / forward_reverse_range;
                goto clamp_and_write;
            case _vehicle_export_velocity_left:
            case _vehicle_export_velocity_up:
                value = __fabs(((up[0] * velocity[0])
                                + ((velocity[2] * up[2]) + (velocity[1] * up[1]))))
                        / forward_reverse_range;
                goto clamp_and_write;
            case _vehicle_export_left_tread_position:
                value = vehicle->vehicle.left_tread / definition->vehicle.wheel_circumference;
                goto clamp_and_write;
            case _vehicle_export_right_tread_position:
                value = vehicle->vehicle.right_tread / definition->vehicle.wheel_circumference;
                goto clamp_and_write;
            case _vehicle_export_left_tread_velocity:
                value = __fabs((throttle - turn)) / forward_reverse_range;
                goto clamp_and_write;
            case _vehicle_export_right_tread_velocity:
                value = __fabs((turn + throttle)) / forward_reverse_range;
                goto clamp_and_write;
            case _vehicle_export_front_left_tire_position:
            case _vehicle_export_front_right_tire_position:
            case _vehicle_export_back_left_tire_position:
            case _vehicle_export_back_right_tire_position:
                value = vehicle->vehicle.wheel / definition->vehicle.wheel_circumference;
                goto clamp_and_write;
            case _vehicle_export_wingtip_contrail:
            {
                real_vector3d parallel;
                real_vector3d perpendicular;
                component_vectors_from_normal3d((const real_vector3d *)velocity,
                        (const real_vector3d *)forward, &parallel, &perpendicular);
                float perpendicular_length = __fsqrts(((perpendicular.n[0] * perpendicular.n[0])
                        + ((perpendicular.n[1] * perpendicular.n[1])
                                + (perpendicular.n[2] * perpendicular.n[2]))));
                float scaled = perpendicular_length * 3.3333333f;
                value = scaled * scaled;
                goto clamp_and_write;
            }
            case _vehicle_export_hover:
                value = vehicle->vehicle.hover;
                goto clamp_and_write;
            case _vehicle_export_thrust:
                value = vehicle->vehicle.thrust;
                goto clamp_and_write;
            case _vehicle_export_engine_hack:
            {
                /* DEVIATION: (float)(unsigned byte) via fcfid; the decompiler's 0x82000000 HIDWORD is the
                 * int->float magic constant, not a real 64-bit value. */
                float gear = (float)vehicle->vehicle.airborne_ticks;
                float throttle_magnitude = __fabs(throttle);
                float blend = (((gear * 0.2f) + 1.0f) * 0.5f);
                float alignment = __fabs(((velocity[0] * forward[0])
                                + ((forward[2] * velocity[2]) + (forward[1] * velocity[1]))))
                        / forward_reverse_range;
                if ( blend < 0.0f )
                    blend = 0.0f;
                else if ( blend > 1.0f )
                    blend = 1.0f;
                value = (((1.0f - blend) * alignment)
                        + (blend * (throttle_magnitude / forward_scale)));
                goto clamp_and_write;
            }
            case _vehicle_export_wingtip_contrail_new:
            {
                float speed = __fsqrts(((velocity[2] * velocity[2])
                        + ((velocity[0] * velocity[0]) + (velocity[1] * velocity[1]))));
                value = ((((speed / definition->vehicle.speed.maximum_forward_speed) * vehicle->vehicle.thrust)
                                - 0.050000001f)
                        * 1.1764706f);
                goto clamp_and_write;
            }
            default:
                goto write_value;

clamp_and_write:
            if ( value >= 0.0f )
            {
                if ( value > 1.0f )
                    value = 1.0f;
            }
            else
            {
                value = 0.0f;
            }
write_value:
            vehicle->object.incoming_function_values[i] = value;
            break;
        }
    }
}
