/* field_properties_network_player_parameters @ 0x84181800 (.data, 116 bytes) — the `parameters` blob for the
 * "structure" (nested field-group) message-delta field type: a member count followed inline by
 * that many _field_reference_definition entries, each naming a nested field's own properties
 * definition plus its byte offset within the destination and baseline structs.
 * DB applied_types: _field_type_structure_parameters field_properties_network_player_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x000 field_count       = 0x00000007 -> 7
 *   +0x004   [ 0] = { 0x84181798 -> &field_properties_network_player_name_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x014   [ 1] = { 0x84183338 -> &field_properties_integer_medium_definition, offset 24, baseline_offset 24, initialized 0 }
 *   +0x024   [ 2] = { 0x84183338 -> &field_properties_integer_medium_definition, offset 26, baseline_offset 26, initialized 0 }
 *   +0x034   [ 3] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 28, baseline_offset 28, initialized 0 }
 *   +0x044   [ 4] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 29, baseline_offset 29, initialized 0 }
 *   +0x054   [ 5] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 30, baseline_offset 30, initialized 0 }
 *   +0x064   [ 6] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 31, baseline_offset 31, initialized 0 }
 * Extent: members_references[] is the trailing flexible array and this symbol is exactly
 * 116 bytes = 0x4 + 7 * 16, i.e. `field_count` live entries and NO terminator. That is a different
 * convention from _message_definition_field_reference_set, which does carry an all-zero terminator:
 * _field_type_structure_parameter_verifier @0x8379B2E8, _field_type_structure_size_calculator
 * @0x8379B250 and default_structure_encoder @0x8379B390 all iterate strictly `i < field_count` and
 * never read a slot past it. Confirmed per object for all 4 structure-parameter blobs.
 */

#include "../headers/field_type_structure_parameters.h"
#include "../headers/field_properties_definition.h"

extern _field_properties_definition field_properties_integer_medium_definition; /* 0x84183338 */
extern _field_properties_definition field_properties_integer_small_definition; /* 0x841832D0 */
extern _field_properties_definition field_properties_network_player_name_definition; /* 0x84181798 */

_field_type_structure_parameters field_properties_network_player_parameters =
{
    7,                                       /* field_count */
    {   /* members_references[field_count]: { properties, offset, baseline_offset, initialized } */
        { &field_properties_network_player_name_definition, 0, 0, 0 },  /* [ 0] */
        { &field_properties_integer_medium_definition, 24, 24, 0 },  /* [ 1] */
        { &field_properties_integer_medium_definition, 26, 26, 0 },  /* [ 2] */
        { &field_properties_integer_small_definition, 28, 28, 0 },  /* [ 3] */
        { &field_properties_integer_small_definition, 29, 29, 0 },  /* [ 4] */
        { &field_properties_integer_small_definition, 30, 30, 0 },  /* [ 5] */
        { &field_properties_integer_small_definition, 31, 31, 0 },  /* [ 6] */
    },
};
