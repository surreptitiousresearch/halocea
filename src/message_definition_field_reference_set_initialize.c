/* message_definition_field_reference_set_initialize @0x837A2568 — one-time lazy initialization of a
 * message's field reference set: for each field reference up to the set's count (capped at 64), lazily
 * initializes the underlying field type's properties definition (verifying its parameters and computing
 * its maximum size) exactly once — shared across every field reference that points at the same
 * properties definition — then marks the reference itself initialized. Bails false if count > 64, if the
 * loop stopped early on an all-zero (padding) reference before reaching count, or if the reference
 * immediately following the last real one isn't itself a valid all-zero terminator. Returns whether
 * initialization fully succeeded. */

#include "headers/message_definition_field_reference_set.h"
#include "headers/field_type_definition.h"
#include "headers/blam_data_globals.h"


int message_definition_field_reference_set_initialize(_message_definition_field_reference_set *const field_reference_set)
{
    int count = field_reference_set->count;
    if ( count > 64 )
        return 0;

    int processed = 0;

    if ( count > 0 )
    {
        _field_reference_definition *reference = field_reference_set->field_references;

        do
        {
            if ( !reference->offset && !reference->baseline_offset && !reference->properties )
                break;

            if ( !reference->initialized )
            {
                _field_properties_definition *properties = reference->properties;
                if ( !properties->initialized )
                {
                    message_delta_global_field_type_list[properties->type].field_parameters_verifier(properties);
                    properties->maximum_size = message_delta_global_field_type_list[properties->type].maximum_size_calculator(properties);
                    properties->initialized = 1;
                }
                reference->initialized = 1;
            }

            count = field_reference_set->count;
            ++processed;
            ++reference;
        }
        while ( processed < field_reference_set->count );
    }

    _field_reference_definition *terminator = &field_reference_set->field_references[processed];
    unsigned __int8 terminator_valid = !(terminator->offset || terminator->baseline_offset || terminator->properties);

    if ( processed != count )
        return 0;

    return terminator_valid;
}
