/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* object_compute_function_values @ 0x836ED828 — evaluate the object's animation "functions" for this
 * frame and store their scalar outputs (used to drive change colors, node animation, shader inputs, etc.).
 * For each function in the definition's functions tag block it builds a time phase (game time / 30,
 * offset per object), evaluates a periodic function, optionally modulates it by an incoming function
 * value, inverts, adds a scaled secondary "wobble" periodic, thresholds, step-quantizes, wraps,
 * adds/multiplies by further incoming values, runs a transition function, scales, maps into
 * [lower_bound,upper_bound] with optional normalize, gates "active" by a bit, and (for turning
 * functions, flag 0x2) accumulates the previous output modulo 1. Results go to
 * outgoing_function_values[i]; the per-function active bits live in functions_active_flags.
 *
 * The 1-based incoming function indices address incoming_function_values[index-1] (base raw offset 292
 * = 4*(index+72) with index >= 1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_function_definition.h"
#include "headers/object_function_flags.h"
#include "headers/object_function_bounds_mode.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int game_time_get(void);
extern float periodic_function_evaluate(int16_t function_type, float time);
extern float transition_function_evaluate(int16_t function_type, float value);
extern double floor(double x);
extern double fmod(double x, double y);

void object_compute_function_values(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);
    /* incoming_function_values[-1] base: the tag's function indices are 1-based */
    float *incoming_values = object->object.incoming_function_values - 1;

    int   time_ticks = game_time_get() + 57 * (uint16_t)object_index;
    float time_seconds = (float)time_ticks * SECONDS_PER_TICK;

    if ( object_definition->functions.count > 0 )
    {
        int16_t i = 0;
        do
        {
            object_function_definition *function =
                (object_function_definition *)object_definition->functions.address + i;
            unsigned char active = 1;

            /* phase = period_scale (optionally divided by an input function value) * time */
            float period_scale = function->runtime_one_over_period;
            if ( function->scale_period_by_function_index )
            {
                int16_t input_value_index = function->scale_period_by_function_index;
                if ( incoming_values[input_value_index] > 0.0f )
                    period_scale = function->runtime_one_over_period / incoming_values[input_value_index];
            }

            float value = periodic_function_evaluate(function->function_type, period_scale * time_seconds);

            if ( function->scale_function_by_function_index )
                value = incoming_values[function->scale_function_by_function_index] * value;
            if ( (function->flags & (1u << _object_function_invert_function_bit)) != 0 )
                value = 1.0f - value;

            if ( function->wobble_magnitude != 0.0f )
            {
                float wobble = periodic_function_evaluate(function->wobble_function_type,
                                                          function->wobble_period * time_seconds);
                value = (wobble - 0.5f) * function->wobble_magnitude * 2.0f + value;
            }

            float threshold = function->square_wave_threshold;
            if ( threshold != 0.0f )
                value = (value <= threshold) ? 0.0f : 1.0f;

            int step_count = function->step_count;
            if ( step_count > 1 )
                value = (float)floor((double)((float)step_count * value)) * function->runtime_reciprocal_step_count;

            if ( function->runtime_reciprocal_sawtooth_count > 0.0f )
                value = (float)fmod((double)value, (double)function->runtime_reciprocal_sawtooth_count);

            if ( function->add_function_index )
            {
                value = incoming_values[function->add_function_index] + value;
                if ( value > 1.0f )
                    value = 1.0f;
            }
            if ( function->scale_result_by_function_index )
                value = incoming_values[function->scale_result_by_function_index] * value;

            float output = transition_function_evaluate(function->map_result_to_transition_function, value);
            if ( function->scale_by > 0.0f )
                output = function->scale_by * output;

            int16_t bounds_mode = function->bounds_mode;
            if ( bounds_mode == _object_function_scale_to_fit_bounds )
            {
                output = (function->upper_bound - function->lower_bound) * output + function->lower_bound;
                if ( output <= function->lower_bound + 0.000099999997f )
                    active = (function->flags >> _object_function_does_not_deactivate_below_lower_bound_bit) & 1;
            }
            else
            {
                if ( output <= function->lower_bound + 0.000099999997f )
                {
                    output = function->lower_bound;
                    active = (function->flags >> _object_function_does_not_deactivate_below_lower_bound_bit) & 1;
                }
                if ( output > function->upper_bound )
                    output = function->upper_bound;
                if ( bounds_mode == 1 )
                    output = (output - function->lower_bound) * function->runtime_reciprocal_bounds_range;
            }

            int16_t active_gate_bit = function->turn_off_with_function_index;
            if ( active_gate_bit != -1 )
                active = ((1 << active_gate_bit) & object->object.functions_active_flags) == 0 ? 0 : active;

            if ( (function->flags & (1u << _object_function_additive_bit)) != 0 )
                output = (float)fmod((double)(object->object.outgoing_function_values[i] + output), 1.0);

            object->object.outgoing_function_values[i] = output;

            unsigned char bit = 1 << i;
            unsigned char active_bits = object->object.functions_active_flags;
            if ( active )
                object->object.functions_active_flags = bit | active_bits;
            else
                object->object.functions_active_flags = active_bits & ~bit;

            i = (int16_t)(i + 1);
        }
        while ( i < object_definition->functions.count );
    }
}
