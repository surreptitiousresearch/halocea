#pragma once
/* _field_type_enumeration_parameters — the `parameters` blob for an enumeration field (12 bytes, DB layout). */

/* DB-verified (types_enum_values _field_width): storage widths for enumeration/flag fields.
 * 0 = 1 byte, 1 = 2 bytes, else 4 bytes (per field_type_enumeration_size_calculator). */
typedef enum _field_width
{
    _field_width_small  = 0,
    _field_width_medium = 1,
    _field_width_large  = 2,
    _field_width_1_bit  = 3,
    _field_width_3_bits = 4,
    _field_width_5_bits = 5,
    _field_width_6_bits = 6,
} _field_width;

typedef struct _field_type_enumeration_parameters
{
    const _field_width width;         /* 0x00 */
    const int           smallest_value; /* 0x04 */
    const int           largest_value;  /* 0x08 */
} _field_type_enumeration_parameters;
