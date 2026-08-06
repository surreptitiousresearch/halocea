/* field_properties_network_map_parameters @ 0x8418169C (.data, 36 bytes) — the `parameters` blob for the
 * "structure" (nested field-group) message-delta field type: a member count followed inline by
 * that many _field_reference_definition entries, each naming a nested field's own properties
 * definition plus its byte offset within the destination and baseline structs.
 * DB applied_types: _field_type_structure_parameters field_properties_network_map_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x000 field_count       = 0x00000002 -> 2
 *   +0x004   [ 0] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x014   [ 1] = { 0x841816C0 -> &field_properties_map_name_definition, offset 4, baseline_offset 4, initialized 0 }
 * Extent: members_references[] is the trailing flexible array and this symbol is exactly
 * 36 bytes = 0x4 + 2 * 16, i.e. `field_count` live entries and NO terminator. That is a different
 * convention from _message_definition_field_reference_set, which does carry an all-zero terminator:
 * _field_type_structure_parameter_verifier @0x8379B2E8, _field_type_structure_size_calculator
 * @0x8379B250 and default_structure_encoder @0x8379B390 all iterate strictly `i < field_count` and
 * never read a slot past it. Confirmed per object for all 4 structure-parameter blobs.
 */

#include "../headers/field_type_structure_parameters.h"
#include "../headers/field_properties_definition.h"

extern _field_properties_definition field_properties_integer_large_definition; /* 0x841833D0 */
extern _field_properties_definition field_properties_map_name_definition;  /* 0x841816C0 */

_field_type_structure_parameters field_properties_network_map_parameters =
{
    2,                                       /* field_count */
    {   /* members_references[field_count]: { properties, offset, baseline_offset, initialized } */
        { &field_properties_integer_large_definition, 0, 0, 0 },  /* [ 0] */
        { &field_properties_map_name_definition, 4, 4, 0 },  /* [ 1] */
    },
};
