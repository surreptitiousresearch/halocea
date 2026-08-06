#pragma once
/* _field_type_integer_parameters — the `parameters` blob for an "integer" message-delta field (4 bytes,
 * DB layout, confirmed by headers_ref `_field_type_integer_parameters` sizeof=0x4): just the storage
 * width, which _field_type_integer_size_calculator turns into a serialized bit count. */

#include "field_type_enumeration_parameters.h"   /* _field_width */

typedef struct _field_type_integer_parameters
{
    const _field_width width;   /* 0x00 */
} _field_type_integer_parameters;
