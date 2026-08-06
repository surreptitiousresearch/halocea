/* field_properties_point3d_parameters @ 0x84183B9C (.data, 4 bytes) — the `parameters` blob for
 * field_properties_point3d: how many real components the point carries.
 * DB applied_types: _field_type_point_parameters field_properties_point3d_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 dimensions      = 0x00000003 -> 3
 */

#include "../headers/field_type_point_parameters.h"

_field_type_point_parameters field_properties_point3d_parameters =
{
    3                                         /* dimensions */
};
