/* field_properties_game_variant_parameters @ 0x8417D820 (.data, 84 bytes) — the `parameters` blob for the
 * "structure" (nested field-group) message-delta field type: a member count followed inline by
 * that many _field_reference_definition entries, each naming a nested field's own properties
 * definition plus its byte offset within the destination and baseline structs.
 * DB applied_types: _field_type_structure_parameters field_properties_game_variant_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x000 field_count       = 0x00000005 -> 5
 *   +0x004   [ 0] = { 0x8417D728 -> &field_properties_game_variant_human_readable_description_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x014   [ 1] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 48, baseline_offset 48, initialized 0 }
 *   +0x024   [ 2] = { 0x8417D658 -> &field_properties_universal_variant_definition, offset 52, baseline_offset 52, initialized 0 }
 *   +0x034   [ 3] = { 0x8417D6C0 -> &field_properties_game_engine_variant_definition, offset 124, baseline_offset 124, initialized 0 }
 *   +0x044   [ 4] = { 0x8417D7B8 -> &field_properties_game_variant_flags_definition, offset 148, baseline_offset 148, initialized 0 }
 * Extent: members_references[] is the trailing flexible array and this symbol is exactly
 * 84 bytes = 0x4 + 5 * 16, i.e. `field_count` live entries and NO terminator. That is a different
 * convention from _message_definition_field_reference_set, which does carry an all-zero terminator:
 * _field_type_structure_parameter_verifier @0x8379B2E8, _field_type_structure_size_calculator
 * @0x8379B250 and default_structure_encoder @0x8379B390 all iterate strictly `i < field_count` and
 * never read a slot past it. Confirmed per object for all 4 structure-parameter blobs.
 */

#include "../headers/field_type_structure_parameters.h"
#include "../headers/field_properties_definition.h"

extern _field_properties_definition field_properties_game_engine_variant_definition; /* 0x8417D6C0 */
extern _field_properties_definition field_properties_game_variant_flags_definition; /* 0x8417D7B8 */
extern _field_properties_definition field_properties_game_variant_human_readable_description_definition; /* 0x8417D728 */
extern _field_properties_definition field_properties_integer_large_definition; /* 0x841833D0 */
extern _field_properties_definition field_properties_universal_variant_definition; /* 0x8417D658 */

_field_type_structure_parameters field_properties_game_variant_parameters =
{
    5,                                       /* field_count */
    {   /* members_references[field_count]: { properties, offset, baseline_offset, initialized } */
        { &field_properties_game_variant_human_readable_description_definition, 0, 0, 0 },  /* [ 0] */
        { &field_properties_integer_large_definition, 48, 48, 0 },  /* [ 1] */
        { &field_properties_universal_variant_definition, 52, 52, 0 },  /* [ 2] */
        { &field_properties_game_engine_variant_definition, 124, 124, 0 },  /* [ 3] */
        { &field_properties_game_variant_flags_definition, 148, 148, 0 },  /* [ 4] */
    },
};
