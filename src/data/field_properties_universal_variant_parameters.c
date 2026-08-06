/* field_properties_universal_variant_parameters @ 0x8417D4D0 (.data, 388 bytes) — the `parameters` blob for the
 * "structure" (nested field-group) message-delta field type: a member count followed inline by
 * that many _field_reference_definition entries, each naming a nested field's own properties
 * definition plus its byte offset within the destination and baseline structs.
 * DB applied_types: _field_type_structure_parameters field_properties_universal_variant_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x000 field_count       = 0x00000018 -> 24
 *   +0x004   [ 0] = { 0x84183960 -> &field_properties_boolean_definition, offset 0, baseline_offset 0, initialized 0 }
 *   +0x014   [ 1] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 1, baseline_offset 1, initialized 0 }
 *   +0x024   [ 2] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 2, baseline_offset 2, initialized 0 }
 *   +0x034   [ 3] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 3, baseline_offset 3, initialized 0 }
 *   +0x044   [ 4] = { 0x8417D468 -> &field_properties_universal_variant_flags_definition, offset 4, baseline_offset 4, initialized 0 }
 *   +0x054   [ 5] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 8, baseline_offset 8, initialized 0 }
 *   +0x064   [ 6] = { 0x84183960 -> &field_properties_boolean_definition, offset 12, baseline_offset 12, initialized 0 }
 *   +0x074   [ 7] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 13, baseline_offset 13, initialized 0 }
 *   +0x084   [ 8] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 14, baseline_offset 14, initialized 0 }
 *   +0x094   [ 9] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 15, baseline_offset 15, initialized 0 }
 *   +0x0A4   [10] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 16, baseline_offset 16, initialized 0 }
 *   +0x0B4   [11] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 20, baseline_offset 20, initialized 0 }
 *   +0x0C4   [12] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 24, baseline_offset 24, initialized 0 }
 *   +0x0D4   [13] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 28, baseline_offset 28, initialized 0 }
 *   +0x0E4   [14] = { 0x84183438 -> &field_properties_real_definition, offset 32, baseline_offset 32, initialized 0 }
 *   +0x0F4   [15] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 36, baseline_offset 36, initialized 0 }
 *   +0x104   [16] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 40, baseline_offset 40, initialized 0 }
 *   +0x114   [17] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 44, baseline_offset 44, initialized 0 }
 *   +0x124   [18] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 48, baseline_offset 48, initialized 0 }
 *   +0x134   [19] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 52, baseline_offset 52, initialized 0 }
 *   +0x144   [20] = { 0x841832D0 -> &field_properties_integer_small_definition, offset 56, baseline_offset 56, initialized 0 }
 *   +0x154   [21] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 60, baseline_offset 60, initialized 0 }
 *   +0x164   [22] = { 0x84183960 -> &field_properties_boolean_definition, offset 64, baseline_offset 64, initialized 0 }
 *   +0x174   [23] = { 0x841833D0 -> &field_properties_integer_large_definition, offset 68, baseline_offset 68, initialized 0 }
 * Extent: members_references[] is the trailing flexible array and this symbol is exactly
 * 388 bytes = 0x4 + 24 * 16, i.e. `field_count` live entries and NO terminator. That is a different
 * convention from _message_definition_field_reference_set, which does carry an all-zero terminator:
 * _field_type_structure_parameter_verifier @0x8379B2E8, _field_type_structure_size_calculator
 * @0x8379B250 and default_structure_encoder @0x8379B390 all iterate strictly `i < field_count` and
 * never read a slot past it. Confirmed per object for all 4 structure-parameter blobs.
 */

#include "../headers/field_type_structure_parameters.h"
#include "../headers/field_properties_definition.h"

extern _field_properties_definition field_properties_boolean_definition;   /* 0x84183960 */
extern _field_properties_definition field_properties_integer_large_definition; /* 0x841833D0 */
extern _field_properties_definition field_properties_integer_small_definition; /* 0x841832D0 */
extern _field_properties_definition field_properties_real_definition;      /* 0x84183438 */
extern _field_properties_definition field_properties_universal_variant_flags_definition; /* 0x8417D468 */

_field_type_structure_parameters field_properties_universal_variant_parameters =
{
    24,                                      /* field_count */
    {   /* members_references[field_count]: { properties, offset, baseline_offset, initialized } */
        { &field_properties_boolean_definition, 0, 0, 0 },  /* [ 0] */
        { &field_properties_integer_small_definition, 1, 1, 0 },  /* [ 1] */
        { &field_properties_integer_small_definition, 2, 2, 0 },  /* [ 2] */
        { &field_properties_integer_small_definition, 3, 3, 0 },  /* [ 3] */
        { &field_properties_universal_variant_flags_definition, 4, 4, 0 },  /* [ 4] */
        { &field_properties_integer_large_definition, 8, 8, 0 },  /* [ 5] */
        { &field_properties_boolean_definition, 12, 12, 0 },  /* [ 6] */
        { &field_properties_integer_small_definition, 13, 13, 0 },  /* [ 7] */
        { &field_properties_integer_small_definition, 14, 14, 0 },  /* [ 8] */
        { &field_properties_integer_small_definition, 15, 15, 0 },  /* [ 9] */
        { &field_properties_integer_large_definition, 16, 16, 0 },  /* [10] */
        { &field_properties_integer_large_definition, 20, 20, 0 },  /* [11] */
        { &field_properties_integer_large_definition, 24, 24, 0 },  /* [12] */
        { &field_properties_integer_large_definition, 28, 28, 0 },  /* [13] */
        { &field_properties_real_definition, 32, 32, 0 },  /* [14] */
        { &field_properties_integer_large_definition, 36, 36, 0 },  /* [15] */
        { &field_properties_integer_large_definition, 40, 40, 0 },  /* [16] */
        { &field_properties_integer_large_definition, 44, 44, 0 },  /* [17] */
        { &field_properties_integer_large_definition, 48, 48, 0 },  /* [18] */
        { &field_properties_integer_large_definition, 52, 52, 0 },  /* [19] */
        { &field_properties_integer_small_definition, 56, 56, 0 },  /* [20] */
        { &field_properties_integer_large_definition, 60, 60, 0 },  /* [21] */
        { &field_properties_boolean_definition, 64, 64, 0 },  /* [22] */
        { &field_properties_integer_large_definition, 68, 68, 0 },  /* [23] */
    },
};
