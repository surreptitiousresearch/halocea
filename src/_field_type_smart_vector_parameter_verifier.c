/* _field_type_smart_vector_parameter_verifier @0x8379D898 — one-time validation/finalization of a
 * smart_vector field type's parameters: NaN-checks min_value (and, only if that passed, max_value —
 * their results aren't otherwise used, this reads as a debug-style validation pass), defaults each
 * transport's value range to the maximum representable by its configured bit width if not explicitly
 * set, then quantizes every configured common_vectors[] entry into squished_common_vectors[] (bailing
 * false if any common vector contains a NaN component). Returns true if there are no common vectors to
 * quantize, or once all of them quantize successfully; false on the first NaN-containing common vector.
 *
 * DEVIATION: the decompiler fabricates a phantom 2nd parameter ("a2"); the DB's real 1-parameter
 * prototype (field_properties_definition) is ground truth — "a2" is just decompiler noise around the
 * (also discarded) _isnan(min_value)/_isnan(max_value) validation calls. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_smart_vector_parameters.h"

#include "headers/quantized_smart_vector.h"
extern int _isnan(double x);
extern void convert_to_quantized_smart(const real_vector3d *const in, quantized_smart_vector *const out, const _field_type_smart_vector_parameters *const parameters);

unsigned __int8 _field_type_smart_vector_parameter_verifier(
    const _field_properties_definition *const field_properties_definition)
{
    _field_type_smart_vector_parameters *parameters =
        (_field_type_smart_vector_parameters *)field_properties_definition->parameters;

    if ( !_isnan(parameters->min_value) )
        _isnan(parameters->max_value);

    if ( !parameters->range_of_values_per_component_internet )
        parameters->range_of_values_per_component_internet = (1 << parameters->number_of_bits_per_component_internet) - 1;
    if ( !parameters->range_of_values_per_component_lan )
        parameters->range_of_values_per_component_lan = (1 << parameters->number_of_bits_per_component_lan) - 1;

    if ( parameters->number_of_common_vectors <= 0 )
        return 1;

    for ( int i = 0; i < parameters->number_of_common_vectors; ++i )
    {
        const real_vector3d *common_vector = &parameters->common_vectors[i];

        if ( _isnan(common_vector->n[0]) || _isnan(common_vector->n[1]) || _isnan(common_vector->n[2]) )
            return 0;

        convert_to_quantized_smart(common_vector, &parameters->squished_common_vectors[i], parameters);
    }

    return 1;
}
