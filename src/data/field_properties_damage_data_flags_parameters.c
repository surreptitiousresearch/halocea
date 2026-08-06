/* field_properties_damage_data_flags_parameters @ 0x8417C940 (.data, 36 bytes) — the `parameters` blob for a
 * "flags" message-delta field. `count` is how many bits of the flags dword the field spans, and
 * flags_present[i] selects which of those bits are actually serialized: default_flags_encoder
 * @0x8379C118 loops `i < count` and writes bit i only where flags_present[i] == 1, so an entry
 * inside 0..count-1 may legitimately be 0 — here 8 of the 8 spanned bits are present.
 * _field_type_flags_size_calculator @0x8379C0A8 returns `count` as the field's maximum size.
 * DB applied_types: _field_type_flags_parameters field_properties_damage_data_flags_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 count             = 0x00000008 -> 8
 *   +0x04 flags_present[32] = 01 01 01 01 01 01 01 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 * The object is 36 bytes; the 4 trailing zero byte(s) up to the next named symbol at
 * 0x8417C968 are inter-symbol alignment padding, not part of it.
 */

#include "../headers/field_type_flags_parameters.h"

_field_type_flags_parameters field_properties_damage_data_flags_parameters =
{
    8,                                        /* count */
    {   /* flags_present[32] */
        1, 1, 1, 1, 1, 1, 1, 1,   /* [ 0.. 7] */
        0, 0, 0, 0, 0, 0, 0, 0,   /* [ 8..15] */
        0, 0, 0, 0, 0, 0, 0, 0,   /* [16..23] */
        0, 0, 0, 0, 0, 0, 0, 0,   /* [24..31] */
    },
};
