#pragma once
/* _field_type_fixed_width_parameters — the `parameters` blob for a fixed-width field (8 bytes, DB layout). */

typedef struct _field_type_fixed_width_parameters
{
    const int number_of_bits;   /* 0x00 */
    const int range_of_values;  /* 0x04 */
} _field_type_fixed_width_parameters;
