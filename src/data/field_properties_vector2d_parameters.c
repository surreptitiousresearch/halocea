/* field_properties_vector2d_parameters @ 0x84183C70 (.data, 4 bytes) — the `parameters` blob for
 * field_properties_vector2d: how many real components the vector carries.
 * DB applied_types: _field_type_vector_parameters field_properties_vector2d_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 dimensions      = 0x00000002 -> 2
 */

#include "../headers/field_type_vector_parameters.h"

_field_type_vector_parameters field_properties_vector2d_parameters =
{
    2                                         /* dimensions */
};
