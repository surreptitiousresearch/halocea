#pragma once
/* _field_type_wide_string_parameters — the `parameters` blob for a "wide string" message-delta field
 * (4 bytes, DB layout, confirmed by headers_ref `_field_type_wide_string_parameters` sizeof=0x4):
 * the maximum length in wide characters the field may serialize. */

typedef struct _field_type_wide_string_parameters
{
    const int maximum_length;   /* 0x00 */
} _field_type_wide_string_parameters;
